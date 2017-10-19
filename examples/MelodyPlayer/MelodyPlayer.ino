#include <TimedAction.h>

/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
#include <IRremote.h>
#include "RTTTL.h"
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int ledR = 2;
int ledY = 3;
int ledG = 4;
int RECV_PIN = 6;
IRrecv irrecv(RECV_PIN);
decode_results results;
const int pinBuzzer = 8;

char melody_tones[] = "IndianaJones:d=4,o=5,b=250:e,8p,8f,8g,8p,2c.6,8p.,d,8p,8e,1f,p.,g,8p,8a,8b,8p,2f.6,p,a,8p,8b,2c6,2d6,2e6,e,8p,8f,8g,8p,1c6,p,d6,8p,8e6,1f6,g,8p,8g,e.6,8p,d6,8p,8g,e.6,8p,d6,8p,8g,f.6,8p,e6,8p,8d6,1c6\0";
//char melody_tones[] = "tetris:d=4,o=5,b=63:8c,32p,8d,32p,8e,32p,8f,32p,8g,32p,8a,32p,8b,32p\0";
RTTTL rtttl = RTTTL(pinBuzzer);

bool isWaiting=false;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(ledR, OUTPUT);     
  pinMode(ledY, OUTPUT);     
  pinMode(ledG, OUTPUT);     
  
  Serial.begin(115200);
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("RemoteLeds :");
  rtttl.setMelody(melody_tones);
}

int ledStatus=0;
void showLed()
{
  if(ledStatus & 0x01)
  {
    digitalWrite(ledR, HIGH);   // turn the LED on (HIGH is the voltage level)
  }
  else
    digitalWrite(ledR, LOW);   // turn the LED on (HIGH is the voltage level)

  if(ledStatus & 0x02)
    digitalWrite(ledY, HIGH);   // turn the LED on (HIGH is the voltage level)
  else
    digitalWrite(ledY, LOW);   // turn the LED on (HIGH is the voltage level)

  if(ledStatus & 0x04)
    digitalWrite(ledG, HIGH);   // turn the LED on (HIGH is the voltage level)
  else
    digitalWrite(ledG, LOW);   // turn the LED on (HIGH is the voltage level)
}
TimedAction ledShow = TimedAction(1000,showLed);


// the loop routine runs over and over again forever:
void loop() {
  ledShow.check();
  rtttl.tick();
  if (irrecv.decode(&results)) {
//    Serial.println("Got something");
//    Serial.println(results.value,HEX);
    switch(results.value)
    {
      case 0xFF42BD:
        Serial.println("changingn led status");
        Serial.println(results.value, HEX);
        
        ledStatus=1;
        break;
      case 0xFF52AD:
        ledStatus=2;
        break;
    }
    irrecv.resume(); // Receive the next value
  }
 
}
