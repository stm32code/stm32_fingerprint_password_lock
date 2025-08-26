#include "git.h"

Data_TypeDef Data_init;						  // 设备数据结构体
Threshold_Value_TypeDef threshold_value_init; // 设备阈值设置结构体
Device_Satte_Typedef device_state_init;		  // 设备状态
// 密码锁
U16 pwd = 0;
U8 Page = 0;
U8 Page_Time = 0;
U8 key_num = 0;

extern U8 Pwd_Num;
extern U8 Pwd[4];

// 获取数据参数
mySta Read_Data(Data_TypeDef *Device_Data)
{

	return MY_SUCCESSFUL;
}
// 初始化
mySta Reset_Threshole_Value(Threshold_Value_TypeDef *Value, Device_Satte_Typedef *device_state)
{
	// 密码
	// device_state->DoorPwd = 1234;
	// 写
	// W_Test();
	// 读
	R_Test();
	// 状态重置
	device_state->Error_Num = 3; // 试错次数

	return MY_SUCCESSFUL;
}
// 更新OLED显示屏中内容
mySta Update_oled_massage()
{
	char str[50];
#if OLED // 是否打开
	if (Data_init.Page == 1 && device_state_init.CheckPwd == 0 && device_state_init.Error_Time == 0)
	{
		sprintf(str, " %d %d %d %d  ", Pwd[0], Pwd[1], Pwd[2], Pwd[3]);
		OLED_ShowCH(28, 2, (unsigned char *)str);
	}

	if (device_state_init.Error_Time > 1) // 锁屏
	{
		OLED_ShowCH(32, 2, "解锁失败");
		sprintf(str, "-----  %d  -----", device_state_init.Error_Time);
		OLED_ShowCH(0, 4, (unsigned char *)str);
	}
	if (device_state_init.Error_Time == 1)
	{
		Page = 0;
		Data_init.Page = 0;
		OLED_Clear();
    OLED_ShowCH(16, 0, "KA键密码验证");
    OLED_ShowCH(16, 2, "KB键添加指纹");
    OLED_ShowCH(16, 4, "KC键删除指纹");
    OLED_ShowCH(16, 6, "KD键验证指纹");
		device_state_init.Error_Num = 3;
		device_state_init.Error_Time = 0;
	}
	// 通用密码
	if (device_state_init.CheckPwd == 2)
	{
		if (device_state_init.GetPwd == device_state_init.DoorPwd || device_state_init.door_state)
		{
			OLED_ShowCH(32, 2, "验证成功");
			device_state_init.Error_Num = 3;
			device_state_init.Door_Time = 10;
			device_state_init.CheckPwd = 1;
			Data_init.people_pwd = 0;
			for (int i = 0; i < Pwd_Num; i++)
			{
				Pwd[i] = 0; // 清空
			}
			Pwd_Num = 0;
		}
	}
	// 验证失败
	else if (device_state_init.CheckPwd == 1)
	{
		if (device_state_init.Error_Num > 0 && device_state_init.door_state == 0)
		{

			device_state_init.BeepOpenTime = 250;
			OLED_ShowCH(32, 2, "验证失败");
			for (int i = 0; i < Pwd_Num; i++)
			{
				Pwd[i] = 0; // 清空
			}
			Pwd_Num = 0;
			delay_ms(1500);
			sprintf(str, " %d %d %d %d  ", Pwd[0], Pwd[1], Pwd[2], Pwd[3]);
			OLED_ShowCH(28, 2, (unsigned char *)str);
			device_state_init.Error_Num--;
			device_state_init.GetPwd = 0;
		}
		if (device_state_init.Error_Num == 0)
		{
			device_state_init.massgae_wanning = 1;
			device_state_init.Error_Time = 120;
			OLED_Clear();
			OLED_ShowCH(32, 2, "解锁失败");
		}
	}

#endif
	return MY_SUCCESSFUL;
}

// 更新设备状态
mySta Update_device_massage()
{

	// 自动模式
	if (Data_init.App)
	{
		switch (Data_init.App)
		{
		case 1:
			OneNet_SendMqtt(1); // 发送数据到APP
			break;
		case 2:
			OneNet_SendMqtt(2); // 发送数据到APP
			break;
		}
		Data_init.App = 0;
	}

	return MY_SUCCESSFUL;
}
// 验证密码并开锁
void Input_Password(void)
{

	U8 key = 0;

	if (device_state_init.Error_Time == 0)
	{
		key = Touch_KeyScan();
		if (key != 0)
		{
			if (key == 10) // A (验证)
			{
				OLED_Clear();
				OLED_ShowCH(32, 0, "密    码");
				OLED_ShowCH(16, 4, "KA减   KC确认");
				OLED_ShowCH(16, 6, "       K#返回");
				Page = 1;
				Data_init.Page = 1;
			}
			else if (key == 11) // B  返回上一级
			{
				OLED_Clear();
				Add_FR(); /*添加指纹*/
				Page = 2;
			}
			else if (key == 12) // C  返回上一级
			{
				OLED_Clear();
				Del_FR(); /*删除指纹*/
				Page = 3;
			}
			else if (key == 16) // D 清空
			{
				OLED_Clear();
				OLED_ShowCH(32, 2, "请按手指");
				OLED_ShowCH(32, 4, "K# 返回");
				press_FR(); /*比对指纹*/
				Page = 4;
			}
			else if (key == 13) // *  返回上一级
			{
			}
			else if (key == 15) // # 确认
			{
			}
			device_state_init.BeepOpenTime = 25;
			printf("key:%02d,%d\r\n", key, Page);
		}
	}
}
// 定时器
void Automation_Close(void)
{
	// 实现1自动
	if (device_state_init.Error_Time > 0)
	{
		device_state_init.Error_Time--;
		if (relay1in == 0)
		{
			relay1out = 1;
		}
		if (relay1in == 1 && device_state_init.Error_Time == 1)
		{
			relay1out = 0;
		}
	}
	if (device_state_init.Door_Time > 0)
	{
		device_state_init.Door_Time--;
		if (relay2in == 0)
		{
			relay2out = 1;
		}
	}
	if (relay2in == 1 && device_state_init.Door_Time == 0)
	{
		relay2out = 0;
		device_state_init.door_state = 0;
	}
	// 反馈密码
	if (device_state_init.CheckPwd > 0)
	{
		device_state_init.CheckPwd--;
		Data_init.App = 1;
	}
}
// 检测按键是否按下
static U8 num_on = 0;
static U8 key_old = 0;
void Check_Key_ON_OFF()
{
	U8 key;
	key = KEY_Scan(1);
	// 与上一次的键值比较 如果不相等，表明有键值的变化，开始计时
	if (key != 0 && num_on == 0)
	{
		key_old = key;
		num_on = 1;
	}
	if (key != 0 && num_on >= 1 && num_on <= Key_Scan_Time) // 25*10ms
	{
		num_on++; // 时间记录器
	}
	if (key == 0 && num_on > 0 && num_on < Key_Scan_Time) // 短按
	{
		switch (key_old)
		{
		case KEY1_PRES:
			printf("Key1_Short\n");

			break;
		case KEY2_PRES:
			printf("Key2_Short\n");

			break;
		case KEY3_PRES:
			printf("Key3_Short\n");

			break;

		default:
			break;
		}
		num_on = 0;
	}
	else if (key == 0 && num_on >= Key_Scan_Time) // 长按
	{
		switch (key_old)
		{
		case KEY1_PRES:
			printf("Key1_Long\n");

			Data_init.App = 1;

			break;
		case KEY2_PRES:
			printf("Key2_Long\n");

			break;
		case KEY3_PRES:
			printf("Key3_Long\n");

			break;
		default:
			break;
		}
		num_on = 0;
	}
}
// 解析json数据
mySta massage_parse_json(char *message)
{

	cJSON *cjson_test = NULL; // 检测json格式
	// cJSON *cjson_data = NULL; // 数据
	const char *massage;
	// 定义数据类型
	u8 cjson_cmd; // 指令,方向

	/* 解析整段JSO数据 */
	cjson_test = cJSON_Parse(message);
	if (cjson_test == NULL)
	{
		// 解析失败
		printf("parse fail.\n");
		return MY_FAIL;
	}

	/* 依次根据名称提取JSON数据（键值对） */
	cjson_cmd = cJSON_GetObjectItem(cjson_test, "cmd")->valueint;
	/* 解析嵌套json数据 */
	// cjson_data = cJSON_GetObjectItem(cjson_test, "data");

	switch (cjson_cmd)
	{
	case 0x01: // 消息包

		device_state_init.DoorPwd = cJSON_GetObjectItem(cjson_test, "set")->valueint;
		Data_init.App = 2;
		W_Test();
		break;
	case 0x02: // 消息包
		device_state_init.door_state = cJSON_GetObjectItem(cjson_test, "door")->valueint;
		if (device_state_init.door_state)
		{
			device_state_init.Error_Num = 3;
			device_state_init.Door_Time = 10;
			device_state_init.CheckPwd = 2;
		}
		else
		{
			device_state_init.CheckPwd = 1;
		}
		device_state_init.BeepOpenTime = 50;
		Data_init.people_pwd = 0;
		break;
	case 0x03: // 数据包

		break;
	case 0x04: // 数据包
		Data_init.App = cjson_cmd + 1;

		break;
	default:
		break;
	}

	/* 清空JSON对象(整条链表)的所有数据 */
	cJSON_Delete(cjson_test);

	return MY_SUCCESSFUL;
}
