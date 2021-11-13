//blynk
#define BLYNK_TEMPLATE_ID "TMPLIuPODnmK"
#define BLYNK_DEVICE_NAME "servo"
#define BLYNK_AUTH_TOKEN "nNUa4YGy2T6gWPw-ZOC_65vAOzz-Cw5Y"
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "pixel";
char pass[] = "d006e121f1d0";
float start;
BlynkTimer timer;
BLYNK_WRITE(V1)
{
  start = param.asInt();

}
void myTimerEvent()
{
  Blynk.virtualWrite(V0, millis() / 1000);
}

//servos
#include <ESP32Servo.h>
Servo myservo01;
Servo myservo02;
Servo myservo03;
int servoPin = 2;
int servoPin02 = 16;
int servoPin03 = 17;
void speed2pwm(float speed, int servo_num);
void movemotor(float forward, float side, float rotate);

//huskylens
#include "HUSKYLENS.h"
#include "SoftwareSerial.h"
HUSKYLENS huskylens;
SoftwareSerial mySerial(21, 22); // RX, TX
//HUSKYLENS green line >> Pin 21; blue line >> Pin 22
int whereIsCenter(int id1_center, int id2_center, int previousCenter);
int id1_center = -1;
int id2_center = -1;
int previousCenter = 160;
int center = 160;

//get time
long currentTime;
long lastGetTime;

void setup() {
  Serial.begin(115200);

  //huskylens
  mySerial.begin(9600);
  while (!huskylens.begin(mySerial))
  {
      Serial.println(F("Begin failed!"));
      Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>Serial 9600)"));
      Serial.println(F("2.Please recheck the connection."));
      delay(100);
  }

  //blynk
  Blynk.begin(auth, ssid, pass);
}




void loop() {
  //blynk
  Blynk.run();
  timer.run();
  Serial.println(start);
  //get time
  currentTime = millis();
  
  //initialize everytime in case tag didn't detected
  id1_center = -1;
  id2_center = -1;

  //get data from huskylens
  if (!huskylens.request()) Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
  else if(!huskylens.isLearned()) Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
  else if(!huskylens.available()) Serial.println(F("No block or arrow appears on the screen!"));
  else
  {
      Serial.println(F("###########"));
      while (huskylens.available())
      {
          HUSKYLENSResult result = huskylens.read();
          if(result.ID == 1){
            id1_center = result.xCenter;
          }else if(result.ID == 2){
            id2_center = result.xCenter;
          }
          lastGetTime = millis();
      }
  }

  //debug
  Serial.println(String()+F("id2:")+id1_center+F("id1:")+id2_center);
  Serial.println(String()+currentTime+F("and")+lastGetTime);
  Serial.println(currentTime-lastGetTime);
  
  //start when switch is on, stop when switch is off
  if(start == 1){
    
    //check if the ship is lost
    if(currentTime-lastGetTime > 10 && start == 1){
      //if the ship is lost, turn the other way as the last turn untill it finds the tag
      //to be coded
      Serial.println("I'm lost");
      Serial.println(String()+F("previousCenter:")+previousCenter);
    }else{
      //if not lost, turn or forward to the tag
      center = whereIsCenter(id1_center,id2_center,previousCenter);
      previousCenter = center;
      Serial.println(String()+F("center:")+center);
      
    }
  }
}




int whereIsCenter(int id1_center, int id2_center, int previousCenter){
  int currentCenter;
  if(id1_center != -1 && id2_center == -1) currentCenter = id1_center;
  else if(id1_center == -1 && id2_center != -1) currentCenter = id2_center;
  else if(id1_center != -1 && id2_center != -1) currentCenter = (id1_center+id2_center)/2;
  else currentCenter = previousCenter;
  return currentCenter;
}
