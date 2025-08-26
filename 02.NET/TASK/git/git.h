#ifndef __GIT__H
#define __GIT__H

//  �豸ʹ�����趨��
#define OLED 1			// �Ƿ�ʹ��OLED
#define NET_SERVE 1		// ƽ̨ѡ��
#define NETWORK_CHAEK 1 // �Ƿ������߼��
#define KEY_OPEN 1		// �Ƿ��������Ͷ̰����
#define USART2_OPEN 0	// �Ƿ�ʹ�ô��ڶ�

// ���ݶ���
typedef unsigned char U8;
typedef signed char S8;
typedef unsigned short U16;
typedef signed short S16;
typedef unsigned int U32;
typedef signed int S32;
typedef float F32;

//  C��
#include "cjson.h"
#include <string.h>
#include <stdio.h>
// ��Ƭ��ͷ�ļ�
#include "sys.h"
#include "usart.h"
// ����Э���
#include "onenet.h"
// �����豸
#include "esp8266.h"
// ������
#include "task.h"
#include "timer.h"
// Ӳ������
#include "delay.h"
#include "usart.h"
#include "git.h"
#include "led.h"
#include "key.h"
#include "timer.h"
#include "flash.h"
#include "TTP229.h"
#include "relay.h"
#include "usart3.h"
#include "as608.h"

#if OLED // OLED�ļ�����
#include "oled.h"
#endif

// ��������Ϣ
#define SSID "NET"		// ·����SSID����
#define PASS "12345678" // ·��������
#if NET_SERVE == 0
// ��Э���������Onenet�ɰ�֧�֣�
#define ServerIP "183.230.40.39" // ������IP��ַ
#define ServerPort 6002			 // ������IP��ַ�˿ں�
#elif NET_SERVE == 1
// ����������ƽ̨��������֧�֣�
#define ServerIP "iot-06z00axdhgfk24n.mqtt.iothub.aliyuncs.com" // ������IP��ַ
#define ServerPort 1883											// ������IP��ַ�˿ں�
#elif NET_SERVE == 2
// EMQXƽ̨��������
#define ServerIP "broker.emqx.io" // ������IP��ַ
#define ServerPort 1883			  // ������IP��ַ�˿ں�
#endif
// �豸��Ϣ
#define PROID "MQTT_Device&h9sjqGAebh6"																// ��ƷID
#define DEVID "h9sjqGAebh6.MQTT_Device|securemode=2,signmethod=hmacsha256,timestamp=1705423262732|" // �豸ID
#define AUTH_INFO "5698d88af9a3a154a1603450d940e0fad4f5792f245187a0ddadec59e251f8bd"				// ��Ȩ��Ϣ
// MQTT���� /broadcast/
#define S_TOPIC_NAME "/broadcast/h9sjqGAebh6/test1" // ��Ҫ���ĵ�����
#define P_TOPIC_NAME "/broadcast/h9sjqGAebh6/test2" // ��Ҫ����������

#define P_TOPIC_CMD "/sys/h9sjbaBQFfh/smart_walking_device/thing/event/property/post"

// �Զ��岼������
typedef enum
{
	MY_TRUE,
	MY_FALSE
} myBool;

// �Զ���ִ�н������
typedef enum
{
	MY_SUCCESSFUL = 0x01, // �ɹ�
	MY_FAIL = 0x00		  // ʧ��

} mySta; // �ɹ���־λ

typedef enum
{
	OPEN = 0x01, // ��
	CLOSE = 0x00 // �ر�

} On_or_Off_TypeDef; // �ɹ���־λ

typedef enum
{
	DERVICE_SEND = 0x00, // �豸->ƽ̨
	PLATFORM_SEND = 0x01 // ƽ̨->�豸

} Send_directino; // ���ͷ���

typedef struct
{
	U8 App;			 // ָ��ģʽ
	U8 Device_State; // ģʽ
	U8 Page;		 // ҳ��
	U8 Error_Time;
	U8 time_cut_page; // ҳ��

	F32 temperatuer; // �¶�
	F32 humiditr;	 // ʪ��
	U8 Flage;		 // ģʽѡ��

	U8 people_pwd; // �������

} Data_TypeDef; // ���ݲ����ṹ��

typedef struct
{

	U16 somg_value;		// ������ֵ
	U16 humi_value;		// ʪ����ֵ
	U16 temp_value;		// �¶���ֵ
	U16 Distance_value; // ������ֵ

} Threshold_Value_TypeDef; // ���ݲ����ṹ��

typedef struct
{
	U8 Error_Num; // �Դ����
	U16 DoorPwd;
	U16 door_state;
	U16 GetPwd;	  // ���������
	U16 CheckPwd; // ���������
	U8 Door_Time; // �Դ����
	U8 Error_Time;
	U8 waning;			// ����
	U8 massgae_wanning; // �绰����
	U16 BeepOpenTime;	// ����������ʱ��

} Device_Satte_Typedef; // ״̬�����ṹ��

// ȫ������
extern Data_TypeDef Data_init;
extern Device_Satte_Typedef device_state_init; // �豸״̬

extern Threshold_Value_TypeDef threshold_value_init; // �豸��ֵ���ýṹ��

// ��ȡ���ݲ���
mySta Read_Data(Data_TypeDef *Device_Data);
// ��ʼ��
mySta Reset_Threshole_Value(Threshold_Value_TypeDef *Value, Device_Satte_Typedef *device_state);
// ����OLED��ʾ��������
mySta Update_oled_massage(void);
// �����豸״̬
mySta Update_device_massage(void);
// ����json����
mySta massage_parse_json(char *message);
// ��֤���벢����
void Check_Key_ON_OFF(void);
// 1S
void Automation_Close(void);
// ��֤���벢����
void Input_Password(void);

#endif
