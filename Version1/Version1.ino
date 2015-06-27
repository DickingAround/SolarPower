#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>
YunServer server;
#define CYCLES 200
#define READINGS 200
//Grain in volts = 0.5; 0.5/10/5*1024 = 10
#define GRAIN 10
#define TIMECUTOFF 2000000

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
 pinMode(7,OUTPUT);
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
 //Take readings every X volts until Y second pass
 int valHigh[READINGS];
 int valLow[READINGS];
 unsigned long time[READINGS];
 String str = "";
 float fValHigh;
 float fValLow;
 int oldDelta = 0;
 int newDelta = 0;
 int i = 0;
 unsigned long startTime = micros();
 turnOnCapacitor();
 while(i < READINGS) {
   time[i] = micros();
   valHigh[i] = analogRead(A0);
   valLow[i] = analogRead(A2);
   //client.println(str + time[i] + "," + oldDelta + "," + valHigh[i] + "," + valLow[i]);
   if(time[i] - startTime > TIMECUTOFF)
     break;
   newDelta = (valHigh[i] - valLow[i]);
   if(newDelta - oldDelta > GRAIN) {
     i++;
     oldDelta = newDelta;
   }
 }
 turnOffCapacitor(); //The cap will drain on it's own
 //Return data
 if(i == 0) {
  client.println("No significant voltage change detected.");
 }
 for(int k = 0; k <= i; k++) {
   fValHigh = valHigh[k]*(5.0 / 1023.0);
   fValLow = valLow[k]*(5.0 / 1023.0);
   client.println(str + time[k] + "\t" + fValHigh + "\t" + fValLow);
 }
}

void turnOnCapacitor() {
 digitalWrite(7,HIGH);
}

void turnOffCapacitor() {
  digitalWrite(7,LOW);
  digitalWrite(5,LOW);
}



/*void process(YunClient client) {
 int valHigh[CYCLES];
 int valLow[CYCLES];
 unsigned long time[CYCLES];
 float fValHigh;
 float fValLow;
 String str = "";
 int waitTime = client.parseInt();
 //Turn on capacitor
 turnOnCapacitor();
 //Measure time to drain
 for(int i; i < CYCLES; i++) {
   time[i] = micros();
   valHigh[i] = analogRead(A0);
   valLow[i] = analogRead(A2);
   if(waitTime > 0)
     delayMicroseconds(waitTime);
 }
 turnOffCapacitor(); //The cap will drain on it's own
 //Return data
 for(int i; i < CYCLES-1; i++) {
   fValHigh = valHigh[i]*(5.0 / 1023.0);
   fValLow = valLow[i]*(5.0 / 1023.0);
   client.println(str + time[i] + "\t" + fValHigh + "\t" + fValLow);
 }
}*/

