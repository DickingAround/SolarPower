#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>
YunServer server;
#define CYCLES 200

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
 pinMode(5,OUTPUT);
 turnOffCapacitor();
 
 //Input to the voltage reader
 pinMode(A0,INPUT);
 pinMode(A2,INPUT);
}
 
void loop() {
  YunClient client = server.accept();
  if(client) {
    process(client);
    client.stop();
  }
  //Turn off all the resistors after every cycle, just to make sure they're off
  turnOffCapacitor();
  delay(200);
}

void process(YunClient client) {
 int valHigh[CYCLES];
 int valLow[CYCLES];
 unsigned long time[CYCLES];
 float fValHigh;
 float fValLow;
 String str = "";
 //Turn on capacitor
 //turnOnCapacitor();
 //Measure time to drain
 for(int i; i < CYCLES; i++) {
   if(i == 10)
     digitalWrite(5,HIGH);
   time[i] = micros();
   valHigh[i] = analogRead(A0);
   valLow[i] = analogRead(A2);
   //delayMicroseconds(100);
 }
 turnOffCapacitor(); //The cap will drain on it's own
 //Return data
 for(int i; i < CYCLES-1; i++) {
   fValHigh = valHigh[i]*(5.0 / 1023.0);
   fValLow = valLow[i]*(5.0 / 1023.0);
   client.println(str + time[i] + "\t" + fValHigh + "\t" + fValLow);
 }
}

void oldPocess(YunClient client) {
  int pin, value;
  float fValue;
  pin = client.parseInt();
  if(pin == 3 || pin == 5 || pin == 7) {
   //Turn on the mosfet connecting that resistor network
   //turnOnResistor(pin);
   //Remove this!!!
   delay(10);
   //Read the value and reply
   value = analogRead(A0);
   fValue = value*(5.0 / 1023.0);
   //client.print(F("Input reads "));
   client.println(fValue);
   //don't need to turn off the resistors because the loop does 
  }
}

void turnOnCapacitor() {
 digitalWrite(5,HIGH);
}

void turnOffCapacitor() {
  digitalWrite(5,LOW);
}

