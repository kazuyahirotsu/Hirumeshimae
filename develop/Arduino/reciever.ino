byte val=0;
#define LED_PIN 13

void setup() { 
   pinMode(LED_PIN, OUTPUT); 
   Serial.begin(9600);
}

void loop() {
   if(Serial.available() > 0){ 
      val = Serial.read();
   }
   if(val == 'a') digitalWrite(LED_PIN, HIGH);
   else if(val == '0') digitalWrite(LED_PIN, LOW);
}