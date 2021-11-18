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
int for_offset = 25;
int back_offset = 35;

//gripper
Servo gripperServo;
int gripperServoPin = 13;
int closedPos = 90;
int openPos = 0;

//huskylens
#include "HUSKYLENS.h"
#include "SoftwareSerial.h"
HUSKYLENS huskylens;
SoftwareSerial mySerial(26, 27); // RX, TX
//HUSKYLENS green line >> Pin 26; blue line >> Pin 27
int whereIsCenter(int id1_center, int id2_center, int previousCenter);
int id1_center = -1;
int id2_center = -1;
int previousCenter = 160;
int center = 160;

//ultrasonic sensor
#include <math.h>
int trigPin = 32;
int echoPin = 33;
int duration;
float distance;

//laser sensor SDA:21 SDL:22
#include <Wire.h>
#include<VL53L0X.h>
VL53L0X laserSensor;
int laserDistance;

//docking
bool success = false;

//get time
long currentTime;
long lastGetTime;
long setupTime;

void setup() {
  Serial.begin(115200);
  setupTime = millis();

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
  timer.setInterval(1000L, myTimerEvent);

  //motor
  myservo01.setPeriodHertz(50);    // standard 50 hz servo
  myservo02.setPeriodHertz(50);
  myservo03.setPeriodHertz(50);
  myservo01.attach(servoPin, 1000, 2000); // attaches the servo on pin 18 to the servo object
  myservo02.attach(servoPin02, 1000, 2000);
  myservo03.attach(servoPin03, 1000, 2000);
  movemotor(0,0,0);

  //gripper
  gripperServo.attach(gripperServoPin);
  gripperServo.write(openPos);
  delay(100);

  //ultrasonic sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //laser sensor
  Wire.begin(SDA, SCL);
  laserSensor.setTimeout(500);
  if (!laserSensor.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1) {}
  }
  laserSensor.startContinuous();

}




void loop() {
  //blynk
  Blynk.run();
  timer.run();
  Serial.println(String()+"start:"+start);
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
  Serial.println(String()+F("id1:")+id1_center+F("  id2:")+id2_center);
  //Serial.println(String()+currentTime+F("and")+lastGetTime);
  //Serial.println(currentTime-lastGetTime);
  
  //start when switch is on, stop when switch is off
  //use movemotor only inside this
  
  if(start == 1){
    //check if the ship is lost
    if((currentTime-setupTime)%30000 == 0 || (currentTime-setupTime)%30000 == 1 || (currentTime-setupTime)%30000 == 2){
      movemotor(-0.1,0,0);
      Serial.println("backing");
      delay(5               00);
      currentTime = millis();
    }
    if(currentTime-lastGetTime > 10 && start == 1){
      //if the ship is lost, turn the other way as the last turn untill it finds the tag
      //to be coded
      Serial.println("I'm lost");
      Serial.println(String()+F("previousCenter:")+previousCenter);
      
      if(previousCenter < 160){
        //rotate counterclockwise if the tag disappeared in the left side
        Serial.println("rotate counterclockwise to get back");
        movemotor(0,0,-0.1);
      }else{
        //rotate clockwise if the tag disappeared in the right side
        Serial.println("rotate clockwise to get back");
        movemotor(0,0,0.1);
      }
      
    }else{
      //if not lost, rotate or forward to the tag
      center = whereIsCenter(id1_center,id2_center,previousCenter);
      previousCenter = center;
      Serial.println(String()+F("center:")+center);
      
      if(100 < center && center < 220){
        Serial.println("go forward");
        movemotor(0.05,0,0);
      }else if(center <= 100){
        Serial.println("rotate counterclockwise");
        movemotor(0,0,-0.05);
      }else{
        Serial.println("rotate clockwise");
        movemotor(0,0,0.05);
      }
    }

    //docking
    //distance = getFarDistance();
    distance = laserSensor.readRangeContinuousMillimeters();
    Serial.println(distance);
    if(distance < 50){
      gripperServo.write(closedPos);
      Serial.println("gripperclose");
      delay(1000);
      laserDistance = laserSensor.readRangeContinuousMillimeters();
      //laserDistance = getFarDistance();
      
      if(laserDistance > 50){
        //unsuccessful docking
        gripperServo.write(openPos);
        delay(1000);
        Serial.println("gripperopen");
        movemotor(-0.1,0,0);
        Serial.println("backing");
        delay(500);
      }else{
        //successful docking
        delay(9000);
        gripperServo.write(openPos);
        delay(1000);
        Serial.println("gripperopen2");
        success = true;
        movemotor(-0.1,0,0);
        Serial.println("backing");
        delay(500);
      }
    }
  }else{
    movemotor(0,0,0);
    gripperServo.write(openPos);
    Serial.println("gripperopen");
    delay(500);
  }
}

int getFarDistance(){
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  duration = pulseIn(echoPin,HIGH);
  distance = duration * 0.034/2;
  Serial.print("distance ");
  Serial.println(distance);
  return distance;
}

int whereIsCenter(int id1_center, int id2_center, int previousCenter){
  int currentCenter;
  if(id1_center != -1 && id2_center == -1) currentCenter = id1_center;
  else if(id1_center == -1 && id2_center != -1) currentCenter = id2_center;
  else if(id1_center != -1 && id2_center != -1) currentCenter = (id1_center+id2_center)/2;
  else currentCenter = previousCenter;
  return currentCenter;
}

void speed2pwm(float speed, int servo_num) {
  int adv;

  adv = speed * 400 + 1500;
  if (speed > 0) adv += for_offset;
  else if (speed < 0) adv -= back_offset;
  else adv = 1500;

  adv = min(max(adv, 1100), 1900);
  if (servo_num == 1) {
    myservo01.writeMicroseconds(adv);
  } else if (servo_num == 2) {
    myservo02.writeMicroseconds(adv);
  } else {
    myservo03.writeMicroseconds(adv);
  }
}
void movemotor(float forward, float side, float rotate) {
  //input is -1<=input<=1
  //if side>0:go right, if rotate>0:go clockwise
  side = -side*0.5;
  forward = -forward*0.5;
  rotate = rotate*0.5;
  float servo1speed = -forward;
  float servo2speed = side + rotate;
  float servo3speed = -side + rotate;

  if(side<0){
    servo3speed = servo3speed*0.2;
    servo2speed = servo2speed*0.7;
  }
  if(rotate){
    servo3speed = servo3speed*0.5;
    servo2speed = servo2speed*0.5;
  }
  speed2pwm(servo1speed, 1);
  speed2pwm(servo2speed, 2);
  speed2pwm(servo3speed, 3);
}
