#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

Servo myservo01;// create servo object to control a servo
Servo myservo02;
Servo myservo03;
// 16 servo objects can be created on the ESP32

int pos = 0;    // variable to store the servo position
// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33 
int servoPin = 2;
int servoPin02 = 16;
int servoPin03 = 17;
void advance(float speed,float turn);

/* Put your SSID & Password */
const char* ssid = "ESP32";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);

uint8_t LED1pin = 4;
bool LED1status = LOW;

uint8_t LED2pin = 5;
bool LED2status = LOW;

void setup() {
  Serial.begin(115200);
  pinMode(LED1pin, OUTPUT);
  pinMode(LED2pin, OUTPUT);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  server.on("/", handle_OnConnect);
  server.on("/led1on", handle_led1on);
  server.on("/led1off", handle_led1off);
  server.on("/led2on", handle_led2on);
  server.on("/led2off", handle_led2off);
  server.onNotFound(handle_NotFound);
  
  server.begin();
  Serial.println("HTTP server started");

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo01.setPeriodHertz(50);    // standard 50 hz servo
  myservo02.setPeriodHertz(50);
  myservo03.setPeriodHertz(50);
  myservo01.attach(servoPin, 1000, 2000); // attaches the servo on pin 18 to the servo object
  myservo02.attach(servoPin02, 1000, 2000);
  myservo03.attach(servoPin03, 1000, 2000);

  advance(0);
}

void advance(float speed){
  int adv1;
  int adv2;
  int adv3;
  
  adv1 = speed*400 + 1500;
  if(speed > 0) adv1 += 25;
  else if(speed < 0) adv1 -= 25;
  else adv1 = 1500;
  adv2 = adv1;
  adv3 = adv1;
  
  adv1 = min(max(adv1,1100),1900);
  adv2 = min(max(adv2,1100),1900);
  adv3 = min(max(adv3,1100),1900);
  
  myservo01.writeMicroseconds(adv1);
  myservo02.writeMicroseconds(adv2);
  myservo03.writeMicroseconds(adv3);
}

void loop() {
  server.handleClient();
  if(LED1status)
  {digitalWrite(LED1pin, HIGH);}
  else
  {digitalWrite(LED1pin, LOW);}
  
  if(LED2status)
  {digitalWrite(LED2pin, HIGH);}
  else
  {digitalWrite(LED2pin, LOW);}

 
}

void handle_OnConnect() {
  LED1status = LOW;
  LED2status = LOW;
  Serial.println("GPIO4 Status: OFF | GPIO5 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,LED2status)); 
}

void handle_led1on() {
  advance(0.3);
  server.send(200, "text/html", SendHTML(true,LED2status)); 
}

void handle_led1off() {
  advance(0);
  server.send(200, "text/html", SendHTML(false,LED2status)); 
}

void handle_led2on() {
  advance(-0.3);
  server.send(200, "text/html", SendHTML(LED1status,true)); 
}

void handle_led2off() {
  advance(0);
  server.send(200, "text/html", SendHTML(LED1status,false)); 
}



void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t led1stat,uint8_t led2stat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #3498db;}\n";
  ptr +=".button-on:active {background-color: #2980b9;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP32 Web Server</h1>\n";
  ptr +="<h3>Using Access Point(AP) Mode</h3>\n";
  
   if(led1stat)
  {ptr +="<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/led1off\">OFF</a>\n";}
  else
  {ptr +="<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/led1on\">ON</a>\n";}

  if(led2stat)
  {ptr +="<p>LED2 Status: ON</p><a class=\"button button-off\" href=\"/led2off\">OFF</a>\n";}
  else
  {ptr +="<p>LED2 Status: OFF</p><a class=\"button button-on\" href=\"/led2on\">ON</a>\n";}


  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
