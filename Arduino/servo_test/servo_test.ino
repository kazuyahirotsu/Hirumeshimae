
#include <WiFi.h>
#include <ESP32Servo.h>

const char ssid[] = "ESP32AP-WiFi";
const char pass[] = "esp32apwifi";
const IPAddress ip(192,168,30,3);
const IPAddress subnet(255,255,255,0);

const char html[] =
"<!DOCTYPE html><html lang='ja'><head><meta charset='UTF-8'>\
<style>input {margin:8px;width:80px;}\
div {font-size:16pt;color:red;text-align:center;width:400px;border:groove 40px orange;}</style>\
<title>WiFi_Car Controller</title></head>\
<body><div><p>Tank Controller</p>\
<form method='get'>\
<input type='submit' name='le' value='左' />\
<input type='submit' name='fo' value='前' />\
<input type='submit' name='ri' value='右' /><br>\
<input type='submit' name='st' value='停止' /><br>\
<input type='submit' name='bl' value='後左' />\
<input type='submit' name='ba' value='後ろ' />\
<input type='submit' name='br' value='後右' /><br><br>\
</form></div></body></html>";

WiFiServer server(80);

Servo myservo01;// create servo object to control a servo
Servo myservo02;
Servo myservo03;
// 16 servo objects can be created on the ESP32

int pos = 0;    // variable to store the servo position
// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33 
int servoPin = 2;
int servoPin02 = 16;
int servoPin03 = 17;
void advance(float speed);
void setup() {
  Serial.begin(38400);
  WiFi.softAP(ssid,pass);
  delay(100);
  WiFi.softAPConfig(ip,ip,subnet);

  IPAddress myIP = WiFi.softAPIP();
	// Allow allocation of all timers
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
	// using default min/max of 1000us and 2000us
	// different servos may require different min/max settings
	// for an accurate 0 to 180 sweep
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  Serial.println("Server start!");
}

void advance(float speed){
  int adv;
  adv = speed*400 + 1500;
  if(speed > 0) adv += 25;
  else if(speed < 0) adv -= 25;
  else adv = 1500;
  
  adv = min(max(adv,1100),1900);
  myservo01.writeMicroseconds(adv);
  myservo02.writeMicroseconds(adv);
  myservo03.writeMicroseconds(adv);
}

void loop() {
   WiFiClient client = server.available();
   Serial.println(client);
    if (client) {
        String currentLine = "";
        Serial.println("New Client.");

        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                Serial.write(c);
                if (c == '\n') {
                    if (currentLine.length() == 0) {
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println();

                        client.print(html);
                        client.println();
                        break;
                    } else {
                        currentLine = "";
                    }
                } else if (c != '\r') {
                    currentLine += c;
                }

                if (currentLine.endsWith("GET /?fo")) {
                    advance(0.3);
                    
                }
               
                if (currentLine.endsWith("GET /?ba")) {
                    advance(-0.3);

                }
            }
        }
        client.stop();
        Serial.println("Client Disconnected.");
    }
    delay(1000);
}
