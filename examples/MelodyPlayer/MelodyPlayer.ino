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

char final_count_down[] = "FinalCountdown:d=4,o=5,b=125:16c#6,16b,c#6,f#,p.,16d6,16c#6,8d6,8c#6,b,p.,16d6,16c#6,d6,f#,p.,16b,16a,8b,8a,8g#,8b,a.,16c#6,16b,c#6,f#,p.,16d6,16c#6,8d6,8c#6,b,p.,16d6,16c#6,d6,f#,p.,16b,16a,8b,8a,8g#,8b,a.,16g#,16a,b.,16a,16b,8c#6,8b,8a,8g#,f#,d6,2c#.6,16c#6,16d6,16c#6,16b,1c#6,2p";
char takeOnMe[] = "TakeOnMe:d=4,o=4,b=160:8f#5,8f#5,8f#5,8d5,8p,8b,8p,8e5,8p,8e5,8p,8e5,8g#5,8g#5,8a5,8b5,8a5,8a5,8a5,8e5,8p,8d5,8p,8f#5,8p,8f#5,8p,8f#5,8e5,8e5,8f#5,8e5,8f#5,8f#5,8f#5,8d5,8p,8b,8p,8e5,8p,8e5,8p,8e5,8g#5,8g#5,8a5,8b5,8a5,8a5,8a5,8e5,8p,8d5,8p,8f#5,8p,8f#5,8p,8f#5,8e5,8e5";
char weRock[] = "we-rock:d=4,o=6,b=80:16a5,16a5,16c6,16e.6,8d.6,4a5,16a5,16a5,16g5,16c.6,4a.5,16a5,16a5,16c6,16e.6,8d.6,4a6,16g6,16g6,16e6,16c.6,2a6,16c7,16a#6,16a6,16g6,16f6,16e6,16d6,16e6,16c6,4d6,16d6,16c7,16a#6,16g#6,16g6,16e6,16d6,16c6,16d6,16e.6,16d6,16c.6,8d6,4a6,16g6,16g6,16e6,16c.6,2d6,32f.5,8f#5,32f.5,32f#.5,16g#5,16f#5,4f.5,32f.5,16f#.5,32f.5,32f#.5,16f5,16d#5,4a#.5,32f.5,8f#5,32f.5,32f#.5,16g#5,16f#5,4f.5,32a5,16a#.5,32a.5,32a#.5,16c#6,16a#5,16g#.5,16f.5,4d5,8a#6,16g#6,16f#6,16f6,16d#6,16f6,16a#5,4d#6,8d#6,16c#6,16b6,16a6,16g#6,16f#6,16d#6,16f.6,16d#6,16c#.6,8d#6,4a#6,16g#6,16g#6,16f6,16c#.6,4d#.6,16g#6,16g#6,16f6,16g#6,16c#6,4d#.6,32f.5,8f#.5,32a#.5,2b5,16c#.5,16c#.5,16g#.5,16d#.5,16a#.5";
char indi[] = "IndianaJones:d=4,o=5,b=250:e,8p,8f,8g,8p,2c.6,8p.,d,8p,8e,1f,p.,g,8p,8a,8b,8p,2f.6,p,a,8p,8b,2c6,2d6,2e6,e,8p,8f,8g,8p,1c6,p,d6,8p,8e6,1f6,g,8p,8g,e.6,8p,d6,8p,8g,e.6,8p,d6,8p,8g,f.6,8p,e6,8p,8d6,1c6";
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
  rtttl.playMelody(final_count_down);
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
  showLed();
  rtttl.tick();
  RTTTL_NoteInfos* note = &rtttl.m_currentNote;
 // printint("address ",(int)&note);
 // Serial.println("");

  if(note->note<5)
      ledStatus = 1;
  else if (note->note<9)
      ledStatus = 2;
  else if (note->note<12)
      ledStatus = 4;
  else
      ledStatus = 0;
  
  if (irrecv.decode(&results)) {
//    Serial.println("Got something");
//    Serial.println(results.value,HEX);
    switch(results.value)
    {
      case 0xFF6897: //0
        rtttl.playMelody(final_count_down);
        break;
      case 0xFF30CF: //1
        rtttl.playMelody(takeOnMe);
        break;
      case 0xFF18E7: //2
        rtttl.playMelody(weRock);
        break;
      case 0xFF7A85: //3
        rtttl.playMelody(indi);
        break;
      case 0xFF10EF: //4
        rtttl.playMelody(indi);
        break;
      case 0xFF38C7: //5
        rtttl.playMelody(indi);
        break;
      case 0xFF5AA5: //6
        rtttl.playMelody(indi);
        break;
      case 0xFF42BD: //7
        rtttl.playMelody(indi);
        break;
      case 0xFF4AB5: //8
        rtttl.playMelody(indi);
        break;
      case 0xFF52AD: //9
        rtttl.playMelody(indi);
        break;
    }
    irrecv.resume(); // Receive the next value
  }
 
}
