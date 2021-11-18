/*************************************************************
​
  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/
​
// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPLIuPODnmK"
#define BLYNK_DEVICE_NAME "servo"
#define BLYNK_AUTH_TOKEN "nNUa4YGy2T6gWPw-ZOC_65vAOzz-Cw5Y"
​
​
// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial
​
​
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
​
char auth[] = BLYNK_AUTH_TOKEN;
​
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "iPhone";
char pass[] = "l2qa0g1j4uhw";
​
​
#include <ESP32Servo.h>
​
Servo myservo01;// create servo object to control a servo
Servo myservo02;
Servo myservo03;
// 16 servo objects can be created on the ESP32
​
int pos = 0;    // variable to store the servo position
// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33 
int servoPin = 2;
​
int servoPin02 = 16;
int servoPin03 = 17;
void advance01(float speed);
void advance02(float speed);
void advance03(float speed);
​
​
BlynkTimer timer;
​
// This function is called every time the Virtual Pin 0 state changes
​
​
BLYNK_WRITE(V1)
{
  float value = param.asFloat();
  advance01(value);
}
​
BLYNK_WRITE(V2)
{
  Serial.println(param.asFloat());
  float value = param.asFloat();
  advance01(value);
}
​
BLYNK_WRITE(V3)
{
  float value = param.asFloat();
  advance01(value);
}
​
// This function is called every time the device is connected to the Blynk.Cloud
​
​
// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V0, millis() / 1000);
}
​
void setup()
{
  // Debug console
  Serial.begin(115200);
​
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
​
  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
​
​
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
 
  myservo01.setPeriodHertz(50);    // standard 50 hz servo
  myservo02.setPeriodHertz(50);
  myservo03.setPeriodHertz(50);
  myservo01.attach(servoPin, 1000, 2000); // attaches the servo on pin 18 to the servo object
  myservo02.attach(servoPin02, 1000, 2000);
  myservo03.attach(servoPin03, 1000, 2000);
​
  advance01(0);
  advance02(0);
  advance03(0);
  
}
bool ​real_test = False;
​
void loop()
{
  Blynk.run();
  timer.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
  distance, argument = from_husky()
​
  
​
  if (real_test){
    // turing
    // left 
    is_detect = True;
    if (not is_detect){
      value_forward = 0.3;
      value_left = 0.3;
      value_right = 0;
    }
    else{
      // keep target center
      keep_center(argument);
      
    }
    // advance ship
​
    
    // use this when u get close to target 
    // input range: -1.0 ~ 1.0
    advance01(value_forward);
    advance02(value_left);
    advance03(value_right);
  }
}
​
void keep_center(argument){
  
}
​
void advance01(float speed){
  int adv;
  
  adv = speed*400 + 1500;
  if(speed > 0) adv += 25;
  else if(speed < 0) adv -= 25;
  else adv = 1500;
  
  adv = min(max(adv,1100),1900);
  
  myservo01.writeMicroseconds(adv);
}
​
void advance02(float speed){
  int adv;
  
  adv = speed*400 + 1500;
  if(speed > 0) adv += 25;
  else if(speed < 0) adv -= 25;
  else adv = 1500;
  
  adv = min(max(adv,1100),1900);
  
  myservo02.writeMicroseconds(adv);
}
void advance03(float speed){
  int adv;
  
  adv = speed*400 + 1500;
  if(speed > 0) adv += 25;
  else if(speed < 0) adv -= 25;
  else adv = 1500;
  
  adv = min(max(adv,1100),1900);
  
  myservo03.writeMicroseconds(adv);
}