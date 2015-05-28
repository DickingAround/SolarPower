#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>
YunServer server;

//The desktop will actually reach out to the Ardunio
//Arduino will set relays to connect resistors
//Then measure the voltage and report back.
void setup() {
 Serial.begin(9600);
 pinMode(13,OUTPUT);
 digitalWrite(13,LOW);
 Bridge.begin();
 digitalWrite(13,HIGH);
 server.listenOnLocalhost();
 server.begin();
 
 //Pins that control the resistor network
 pinMode(4,OUTPUT);
 pinMode(5,OUTPUT);
 pinMode(6,OUTPUT);
 
 //Input to the voltage reader
 pinMode(A0,INPUT);
}
 
void loop() {
  YunClient client = server.accept();
  if(client) {
    process(client);
    client.stop();
  }
  //Turn off all the resistors after every cycle, just to make sure they're off
  turnOffResistor(1);
  turnOffResistor(2);
  turnOffResistor(3);
  delay(200);
}

void process(YunClient client) {
  int pin, value;
  float fValue;
  pin = client.parseInt();
  if(pin >= 4 and pin < 6) {
   //Turn on the mosfet connecting that resistor network
   turnOnResistor(pin);
   //Read the value and reply
   value = analogRead(A0);
   fValue = value*(5.0 / 1023.0);
   //client.print(F("Input reads "));
   client.println(fValue);
   //don't need to turn off the resistors because the loop does 
  }
}

void turnOnResistor(int pin) {
  digitalWrite(pin,HIGH);
}
void turnOffResistor(int pin) {
  digitalWrite(pin,LOW);
}
