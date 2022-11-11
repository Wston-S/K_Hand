#include <Arduino.h>
#include <WiFi.h>
#include "./../inc/net_task.h"
#include <PubSubClient.h>
#include <cJSON.h>

static TaskHandle_t netTask_Handle;

#if 0
static const char *ssid = "504";                    //�����������
static const char *password = "Luxshare@qq.com123"; //�����������
#endif

#if 1
static const char *ssid = "ZTE_5GCPE_5BA0"; //�����������
static const char *password = "3CWA5566G8"; //�����������
#endif

static const long gmtOffset_sec = 8 * 3600;
static const int daylightOffset_sec = 0;
static const char *ntpServer = "pool.ntp.org";

#if 0  //EMQ
// MQTT Broker
const char *mqtt_broker = "broker-cn.emqx.io";
const char *topic = "esp32/007";
const char *mqtt_username = "kuver";
const char *mqtt_password = "nzdhbb";
const int mqtt_port = 1883;
#endif

/*
�豸֤��:
{
  "ProductKey": "a1fMCpcrMAo",
  "DeviceName": "k_hand_devive_001",
  "DeviceSecret": "84fb6059fb0158baee079beebad37dd8"
}

MQTT ���Ӳ���:
{
    "clientId":"a1fMCpcrMAo.k_hand_devive_001|securemode=2,signmethod=hmacsha256,timestamp=1665987500256|",
    "username":"k_hand_devive_001&a1fMCpcrMAo",
    "mqttHostUrl":"a1fMCpcrMAo.iot-as-mqtt.cn-shanghai.aliyuncs.com",
    "passwd":"91d66ea74af22d33f557c51509c1663cbc463de953282845fc91bc595ea582ae",
    "port":1883
}
*/

/*Broker Address(host)��${YourProductKey}.iot-as-mqtt.${YourRegionId}.aliyuncs.com*/
#define Aliyun_host "a1fMCpcrMAo.iot-as-mqtt.cn-shanghai.aliyuncs.com"
#define Aliyun_port 1883 /*�̶�*/
/*Client ID��     ${ClientID}|securemode=${Mode},signmethod=${SignMethod}|*/
#define Aliyun_client_id "a1fMCpcrMAo.k_hand_devive_001|securemode=2,signmethod=hmacsha256,timestamp=1665987500256|"
/*User Name��     ${DeviceName}&${ProductKey}*/
#define Aliyun_username "k_hand_devive_001&a1fMCpcrMAo"
/* passwd */
#define Aliyun_password "91d66ea74af22d33f557c51509c1663cbc463de953282845fc91bc595ea582ae"

#define Aliyun_Body_Format      "{\"params\":%s}"

/* Topic */
#define Aliyun_Topic_UPDATE     "/a1fMCpcrMAo/k_hand_devive_001/user/update"
#define Aliyun_Topic_GET        "/a1fMCpcrMAo/k_hand_devive_001/user/get"

#define Aliyun_Topic_Post        "/sys/a1fMCpcrMAo/k_hand_devive_001/thing/event/property/post"
#define Aliyun_Topic_Post_Reply "/sys/a1fMCpcrMAo/k_hand_devive_001/thing/event/property/post_reply"

WiFiClient espClient;
PubSubClient client(espClient);

static void printLocalTime()
{
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        Serial.println("Failed to obtain time");
        return;
    }
    Serial.println(&timeinfo, "%F %T %A"); // ��ʽ�����
}

static void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    for (int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
    }
    Serial.println();
    Serial.println("-----------------------");
}

static void mqttIntervalPost()
{
    boolean d;
    static char buf[512];
    static float temp = 22.5;

    temp += 0.1;
#if 0
    sprintf(buf, "{\"params\":{\"ESP_MODULE:ESP_SELF\" : \"%s\", \"ESP_MODULE:NightLightSwitch\" : %d,\"ESP_MODULE:Temperature\" : %.1f}}", 
        "this esp_self", 1, temp);
#endif

    sprintf(buf, "{\"sta\":%0.1f}", temp);
    
    d = client.publish(Aliyun_Topic_UPDATE, buf);
    if (true == d)
    {
        Serial.printf("d = %d, publish success!\n", d);
    }
    else
    {
        Serial.printf("d = %d, publichs failed!\n", d);
    }

}

static void mqttCheckConnect()
{
    while (!client.connected())
    {    
        Serial.println("Connecting to MQTT Server ...");
        if (client.connect(Aliyun_client_id, Aliyun_username, Aliyun_password))
        {
            Serial.println("MQTT Connected!");

            bool d = client.subscribe(Aliyun_Topic_GET); 
            if (true == d)
            {
                Serial.printf("%s subscribe success!\n", "--");                
            }
            else
            {
                Serial.printf("%s subscribe failed!\n", "--");
            }

            client.setCallback(callback);
        }
        else
        {
            Serial.print("MQTT Connect err:");
            Serial.println(client.state());
            delay(5000);
        }
    }
}

static void net_task(void *param)
{
    (void)param;

    Serial.printf("net_task start to run\n");

    while (1)
    {
        if (WiFi.isConnected() == true)
        {           
            mqttCheckConnect();            
        }
        else
        {
            Serial.println("WiFi disconnected!");
        }

        {
            static unsigned long pre = 0;

            if(millis() - pre > 1000)
            {
                pre = millis();
                mqttIntervalPost();
            }
        }

        client.loop();
        //vTaskDelay(2000 / portTICK_RATE_MS);
    }
}

static void netTask_init(void)
{
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        static int cnt = 20;
        delay(500);
        Serial.print(".");
        if (0 == cnt--)
            break;
    }

    if (WiFi.isConnected() == true)
    {
        Serial.println("WiFi connected!");
        Serial.println("Connected to AP");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());

        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
        printLocalTime();
    }
    else
    {
        Serial.println("[error]--WiFi un-connected!");
    }

    client.setServer(Aliyun_host, Aliyun_port); /* ����WiFi֮������MQTT������ */
    client.setCallback(callback);
}

void net_task_init(void)
{
    netTask_init();

    xTaskCreate(
        net_task,         /* ������ */
        "net_task",       /* ������ */
        10 * 1024,        /* ����ջ��С��������Ҫ��������*/
        NULL,             /* ���������Ϊ�� */
        3,                /* ���ȼ� */
        &netTask_Handle); /* ������ */
}

/*  
1��esp32 mqttЭ��,���Ӱ�����ʵ��publish �� subsribe��Ϣ,����ʹ�ð����Ƶ�����ģ���ֵ�����,
    ������ʹ��emqttd���з�������ʹ��,�����ѵ�,ʹ��Ҳ�ܷ���,


2������rtthread��˫������,��ʵ����ͨ������˫��������ʹ��  rt_container_of(node, type, member)������������ݽṹ�����


*/

