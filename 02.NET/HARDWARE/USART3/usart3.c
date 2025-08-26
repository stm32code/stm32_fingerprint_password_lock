#include "delay.h"
#include "USART3.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"
#include "timer.h"

// 串口接收缓存区
u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; // 接收缓冲,最大USART3_MAX_RECV_LEN个字节.
u8 USART3_TX_BUF[USART3_MAX_SEND_LEN]; // 发送缓冲,最大USART3_MAX_SEND_LEN字节

// 通过判断接收连续2个字符之间的时间差不大于10ms来决定是不是一次连续的数据.
// 如果2个字符接收间隔超过10ms,则认为不是1次连续数据.也就是超过10ms没有接收到
// 任何数据,则表示此次接收完毕.
// 接收到的数据状态
//[15]:0,没有接收到数据;1,接收到了一批数据.
//[14:0]:接收到的数据长度
vu16 USART3_RX_STA = 0;

void USART3_IRQHandler(void)
{
	u8 res;
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) // 接收到数据
	{
		res = USART_ReceiveData(USART3);
		if ((USART3_RX_STA & (1 << 15)) == 0) // 接收完的一批数据,还没有被处理,则不再接收其他数据
		{
			if (USART3_RX_STA < USART3_MAX_RECV_LEN) // 还可以接收数据
			{
				TIM_SetCounter(TIM3, 0); // 计数器清空          				//计数器清空
				if (USART3_RX_STA == 0)	 // 使能定时器7的中断
				{
					TIM_Cmd(TIM3, ENABLE); // 使能定时器7
				}
				USART3_RX_BUF[USART3_RX_STA++] = res; // 记录接收到的值
			}
			else
			{
				USART3_RX_STA |= 1 << 15; // 强制标记接收完成
			}
		}
	}
}

// 初始化IO 串口2
// pclk1:PCLK1时钟频率(Mhz)
// bound:波特率
void USART3_Init(u32 bound)
{

	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  // GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); // 串口2时钟使能

	USART_DeInit(USART3);					  // 复位串口2
											  // USART3_TX   PA2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; // PA2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);			// 初始化PA2

	// USART3_RX	  PA3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);				  // 初始化PA3

	USART_InitStructure.USART_BaudRate = bound;										// 波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						// 字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							// 一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								// 无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					// 收发模式

	USART_Init(USART3, &USART_InitStructure); // 初始化串口2

	USART_Cmd(USART3, ENABLE); // 使能串口

	// 使能接收中断
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // 开启中断

	// 设置中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; // 抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		  // 子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);							  // 根据指定的参数初始化VIC寄存器

	TIM3_Int_Init(99, 7199); // 10ms中断
	USART3_RX_STA = 0;		 // 清零
	TIM_Cmd(TIM3, DISABLE);	 // 关闭定时器7
}

// 串口2,printf 函数
// 确保一次发送数据不超过USART3_MAX_SEND_LEN字节
void u3_printf(char *fmt, ...)
{
	u16 i, j;
	va_list ap;
	va_start(ap, fmt);
	vsprintf((char *)USART3_TX_BUF, fmt, ap);
	va_end(ap);
	i = strlen((const char *)USART3_TX_BUF); // 此次发送数据的长度
	for (j = 0; j < i; j++)					 // 循环发送数据
	{
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			; // 循环发送,直到发送完毕
		USART_SendData(USART3, USART3_TX_BUF[j]);
	}
}

// 通用定时器3中断初始化
// 这里时钟选择为APB1的2倍，而APB1为36M
// arr：自动重装值。
// psc：时钟预分频数
// 这里使用的是定时器3!
void TIM3_Int_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); // 时钟使能

	// 定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr;						// 设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler = psc;					// 设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		// 设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);				// 根据指定的参数初始化TIMx的时间基数单位

	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); // 使能指定的TIM3中断,允许更新中断
	TIM_Cmd(TIM3, ENABLE);					   // 使能TIMx

	// 中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;			  // TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // 先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		  // 从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);							  // 初始化NVIC寄存器
}

// 定时器3中断服务程序
void TIM3_IRQHandler(void) // TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) // 检查TIM3更新中断发生与否
	{
		USART3_RX_STA |= 1 << 15;					// 标记接收完成
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update); // 清除TIM7更新中断标志
		TIM_Cmd(TIM3, DISABLE);						// 关闭TIM7
	}
}
