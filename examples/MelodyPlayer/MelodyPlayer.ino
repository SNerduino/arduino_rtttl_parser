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

char melody_tones[] = "Dr Who:d=4,o=6,b=80:b.5,8g5,16b.5,8a.5,32g.5,32f#5,g.5,e,32d,32c,8d,8g5,8e.,32d,32c,8d,8b5,2a5,32g5,32f#5,2g5\0";
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
