package com.example.wirelesscombinationlock.utils;

import com.example.wirelesscombinationlock.bean.Send;
import com.example.wirelesscombinationlock.dao.User;
import com.google.gson.Gson;
import com.itfitness.mqttlibrary.MQTTHelper;

public class Common {
    public static String Port = "1883";
    public static String Sever = "tcp://iot-06z00axdhgfk24n.mqtt.iothub.aliyuncs.com" + ":" + Port;
//    public static String Port = "1883";
//    public static String Sever = "tcp://192.168.135.78" + ":" + Port;

    public static String ReceiveTopic = "/broadcast/h9sjqGAebh6/test2";
    public static String PushTopic = "/broadcast/h9sjqGAebh6/test1";
    public static String DriveID = "h9sjqGAebh6.MQTT_APP|securemode=2,signmethod=hmacsha256,timestamp=1705423229764|";
    public static String DriveName = "MQTT_APP&h9sjqGAebh6";
    public static String DrivePassword = "0830f2501ac92055a13989a2170730a2521c8ad7bfbd9b20decf1953e43b6ee7";
    public static String Drive2ID = "1181073142";
    public static String api_key = "EtpuIH6FIU3XjavzoDRPSdYAkT8=";
    public static boolean DeviceOnline = false;
    public static String LatestOnlineDate = "离线";
    public static MQTTHelper mqttHelper = null;

    public static User user = null;
    public static String Lock = null;

    /**
     * @param message 需要发送的消息
     * @brief 再次封装MQTT发送
     */
    public static void sendMessage(int cmd, String... message) {
        if (Common.mqttHelper != null && Common.mqttHelper.getConnected()) {
            Send send = new Send();
            switch (cmd) {
                case 1:
                    send.setSet(Integer.valueOf(message[0]));
                    break;
                case 2:
                    send.setDoor(Integer.parseInt(message[0]));
                    break;
            }
            send.setCmd(cmd);
            String str = new Gson().toJson(send);
            new Thread(() -> Common.mqttHelper.publish(PushTopic, str, 1)).start();
        }
    }
}
