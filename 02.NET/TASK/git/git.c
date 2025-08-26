#include "git.h"

Data_TypeDef Data_init;						  // �豸���ݽṹ��
Threshold_Value_TypeDef threshold_value_init; // �豸��ֵ���ýṹ��
Device_Satte_Typedef device_state_init;		  // �豸״̬
// ������
U16 pwd = 0;
U8 Page = 0;
U8 Page_Time = 0;
U8 key_num = 0;

extern U8 Pwd_Num;
extern U8 Pwd[4];

// ��ȡ���ݲ���
mySta Read_Data(Data_TypeDef *Device_Data)
{

	return MY_SUCCESSFUL;
}
// ��ʼ��
mySta Reset_Threshole_Value(Threshold_Value_TypeDef *Value, Device_Satte_Typedef *device_state)
{
	// ����
	// device_state->DoorPwd = 1234;
	// д
	// W_Test();
	// ��
	R_Test();
	// ״̬����
	device_state->Error_Num = 3; // �Դ����

	return MY_SUCCESSFUL;
}
// ����OLED��ʾ��������
mySta Update_oled_massage()
{
	char str[50];
#if OLED // �Ƿ��
	if (Data_init.Page == 1 && device_state_init.CheckPwd == 0 && device_state_init.Error_Time == 0)
	{
		sprintf(str, " %d %d %d %d  ", Pwd[0], Pwd[1], Pwd[2], Pwd[3]);
		OLED_ShowCH(28, 2, (unsigned char *)str);
	}

	if (device_state_init.Error_Time > 1) // ����
	{
		OLED_ShowCH(32, 2, "����ʧ��");
		sprintf(str, "-----  %d  -----", device_state_init.Error_Time);
		OLED_ShowCH(0, 4, (unsigned char *)str);
	}
	if (device_state_init.Error_Time == 1)
	{
		Page = 0;
		Data_init.Page = 0;
		OLED_Clear();
    OLED_ShowCH(16, 0, "KA��������֤");
    OLED_ShowCH(16, 2, "KB�����ָ��");
    OLED_ShowCH(16, 4, "KC��ɾ��ָ��");
    OLED_ShowCH(16, 6, "KD����ָ֤��");
		device_state_init.Error_Num = 3;
		device_state_init.Error_Time = 0;
	}
	// ͨ������
	if (device_state_init.CheckPwd == 2)
	{
		if (device_state_init.GetPwd == device_state_init.DoorPwd || device_state_init.door_state)
		{
			OLED_ShowCH(32, 2, "��֤�ɹ�");
			device_state_init.Error_Num = 3;
			device_state_init.Door_Time = 10;
			device_state_init.CheckPwd = 1;
			Data_init.people_pwd = 0;
			for (int i = 0; i < Pwd_Num; i++)
			{
				Pwd[i] = 0; // ���
			}
			Pwd_Num = 0;
		}
	}
	// ��֤ʧ��
	else if (device_state_init.CheckPwd == 1)
	{
		if (device_state_init.Error_Num > 0 && device_state_init.door_state == 0)
		{

			device_state_init.BeepOpenTime = 250;
			OLED_ShowCH(32, 2, "��֤ʧ��");
			for (int i = 0; i < Pwd_Num; i++)
			{
				Pwd[i] = 0; // ���
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
			OLED_ShowCH(32, 2, "����ʧ��");
		}
	}

#endif
	return MY_SUCCESSFUL;
}

// �����豸״̬
mySta Update_device_massage()
{

	// �Զ�ģʽ
	if (Data_init.App)
	{
		switch (Data_init.App)
		{
		case 1:
			OneNet_SendMqtt(1); // �������ݵ�APP
			break;
		case 2:
			OneNet_SendMqtt(2); // �������ݵ�APP
			break;
		}
		Data_init.App = 0;
	}

	return MY_SUCCESSFUL;
}
// ��֤���벢����
void Input_Password(void)
{

	U8 key = 0;

	if (device_state_init.Error_Time == 0)
	{
		key = Touch_KeyScan();
		if (key != 0)
		{
			if (key == 10) // A (��֤)
			{
				OLED_Clear();
				OLED_ShowCH(32, 0, "��    ��");
				OLED_ShowCH(16, 4, "KA��   KCȷ��");
				OLED_ShowCH(16, 6, "       K#����");
				Page = 1;
				Data_init.Page = 1;
			}
			else if (key == 11) // B  ������һ��
			{
				OLED_Clear();
				Add_FR(); /*���ָ��*/
				Page = 2;
			}
			else if (key == 12) // C  ������һ��
			{
				OLED_Clear();
				Del_FR(); /*ɾ��ָ��*/
				Page = 3;
			}
			else if (key == 16) // D ���
			{
				OLED_Clear();
				OLED_ShowCH(32, 2, "�밴��ָ");
				OLED_ShowCH(32, 4, "K# ����");
				press_FR(); /*�ȶ�ָ��*/
				Page = 4;
			}
			else if (key == 13) // *  ������һ��
			{
			}
			else if (key == 15) // # ȷ��
			{
			}
			device_state_init.BeepOpenTime = 25;
			printf("key:%02d,%d\r\n", key, Page);
		}
	}
}
// ��ʱ��
void Automation_Close(void)
{
	// ʵ��1�Զ�
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
	// ��������
	if (device_state_init.CheckPwd > 0)
	{
		device_state_init.CheckPwd--;
		Data_init.App = 1;
	}
}
// ��ⰴ���Ƿ���
static U8 num_on = 0;
static U8 key_old = 0;
void Check_Key_ON_OFF()
{
	U8 key;
	key = KEY_Scan(1);
	// ����һ�εļ�ֵ�Ƚ� �������ȣ������м�ֵ�ı仯����ʼ��ʱ
	if (key != 0 && num_on == 0)
	{
		key_old = key;
		num_on = 1;
	}
	if (key != 0 && num_on >= 1 && num_on <= Key_Scan_Time) // 25*10ms
	{
		num_on++; // ʱ���¼��
	}
	if (key == 0 && num_on > 0 && num_on < Key_Scan_Time) // �̰�
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
	else if (key == 0 && num_on >= Key_Scan_Time) // ����
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
// ����json����
mySta massage_parse_json(char *message)
{

	cJSON *cjson_test = NULL; // ���json��ʽ
	// cJSON *cjson_data = NULL; // ����
	const char *massage;
	// ������������
	u8 cjson_cmd; // ָ��,����

	/* ��������JSO���� */
	cjson_test = cJSON_Parse(message);
	if (cjson_test == NULL)
	{
		// ����ʧ��
		printf("parse fail.\n");
		return MY_FAIL;
	}

	/* ���θ���������ȡJSON���ݣ���ֵ�ԣ� */
	cjson_cmd = cJSON_GetObjectItem(cjson_test, "cmd")->valueint;
	/* ����Ƕ��json���� */
	// cjson_data = cJSON_GetObjectItem(cjson_test, "data");

	switch (cjson_cmd)
	{
	case 0x01: // ��Ϣ��

		device_state_init.DoorPwd = cJSON_GetObjectItem(cjson_test, "set")->valueint;
		Data_init.App = 2;
		W_Test();
		break;
	case 0x02: // ��Ϣ��
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
	case 0x03: // ���ݰ�

		break;
	case 0x04: // ���ݰ�
		Data_init.App = cjson_cmd + 1;

		break;
	default:
		break;
	}

	/* ���JSON����(��������)���������� */
	cJSON_Delete(cjson_test);

	return MY_SUCCESSFUL;
}
