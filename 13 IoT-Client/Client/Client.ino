#include "ESP8266.h"
//////////////////////////////////////////////////////////////////////////////// 
// ESP8266 WiFi 모듈을 사용하기 위해
//////////////////////////////////////////////////////////////////////////////// 
#include <SoftwareSerial.h>
//////////////////////////////////////////////////////////////////////////////// 
// ** SoftwareSerial                                                             
//      아두이노의 Serial 포트가 모자랄 때를 대비한 기본 라이브러리                      
//      일반 입출력 데이터 핀을 PX/RX로 작동(이를 소프트웨어 시리얼 <--> 하드웨어 시리얼)   
//      ATmega328  : all pin
//      ATmega2560 : 10-15, 50-53, 62-39 pin
//      ATmega32u4 : 8-11, 14-16 pin
//////////////////////////////////////////////////////////////////////////////////


#define SSID        "jangLab"       // 공유기의 SSID
#define PASSWORD    "emsys2019"     // 공유기의 비밀번호
#define HOST_NAME   "192.168.0.55"  // 컴퓨터의 IP
#define HOST_PORT   8877          // TCP 서버 포트번호

#define LIGHT A3

#define MY_ID       " %d %d "

SoftwareSerial mySerial(8, 9); /*RX:8, TX:9*/
ESP8266 wifi(mySerial);

void setup(void)
{
    Serial.begin(9600);
    Serial.print("setup begin\r\n");

    pinMode(LIGHT, INPUT);
             
    if (wifi.setOprToStationSoftAP()) {
        Serial.print("to station + softap ok\r\n");
    } else {
        Serial.print("to station + softap err\r\n");
    }

    if (wifi.joinAP(SSID, PASSWORD)) {
        Serial.print("Join AP success\r\n");
        Serial.print("IP:");
        Serial.println( wifi.getLocalIP().c_str());       
    } else {
        Serial.print("Join AP failure\r\n");
    }
    
    if (wifi.disableMUX()) {
        Serial.print("single ok\r\n");
    } else {
        Serial.print("single err\r\n");
    }
    
    Serial.print("setup end\r\n");

    if (wifi.createTCP(HOST_NAME, HOST_PORT)) {
        Serial.print("create tcp ok\r\n");
    } else {
        Serial.print("create tcp err\r\n");
    }
}
int cnt = 0;
void loop(void)
{
    uint8_t buffer[128] = {0};

    int value = analogRead(LIGHT);
    int data = map(value, 0, 1023, 1023, 0);

    char _data[10] = "123";
    sprintf(_data, MY_ID, cnt++, data);
    Serial.println(data);
    Serial.println(_data);
    
    wifi.send((const uint8_t*)_data, strlen(_data));
    
//      uint32_t len = wifi.recv(buffer, sizeof(buffer), 10000);a
//      if (len > 0) {
//          Serial.print("Received:[");
//          for(uint32_t i = 0; i < len; i++) {
//              Serial.print((char)buffer[i]);
//          }
//          Serial.print("]\r\n");
//      }
    
//    if (wifi.releaseTCP()) {
//        Serial.print("release tcp ok\r\n");
//    } else {
//        Serial.print("release tcp err\r\n");
//    }
//    delay(100);
    delay(100);
}
