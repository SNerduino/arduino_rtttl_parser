#include <TimedAction.h>

/*
  Sketch : MelodyPlayer
  Author : Saifeddine ALOUI
  This is a simple RTTTL melody player to illustrate how useful it is to use this library in a multitask invironment
  The player setup needs a ELEGOO remote control, 4 leds +  4 resistances 220hms + a buzzer + 300Ohms resistor + infrared receiver
  
  The player is remote controlled using the infrared remote control.
  The parsed notes are used to play the leds.

  3 activities are running in the same time : 
    - our RTTTL library
    - the led switching
    - the remote control signal decoding
  
  This setup was tested on the arduino nano using the ATmega328 microcontroller 
  
*/
#include <IRremote.h>
#include "RTTTL.h"
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int ledR = 2;
int ledY = 3;
int ledG = 4;
int ledB = 5;
int RECV_PIN = 6;
IRrecv irrecv(RECV_PIN);
decode_results results;
const int pinBuzzer = 8;
// ===================================================================================
// Melodies
// ===================================================================================
//char final_count_down[] = "FinalCountdown:d=4,o=5,b=125:16c#6,16b,c#6,f#,p.,16d6,16c#6,8d6,8c#6,b,p.,16d6,16c#6,d6,f#,p.,16b,16a,8b,8a,8g#,8b,a.,16c#6,16b,c#6,f#,p.,16d6,16c#6,8d6,8c#6,b,p.,16d6,16c#6,d6,f#,p.,16b,16a,8b,8a,8g#,8b,a.,16g#,16a,b.,16a,16b,8c#6,8b,8a,8g#,f#,d6,2c#.6,16c#6,16d6,16c#6,16b,1c#6,2p";
//char takeOnMe[] = "TakeOnMe:d=4,o=4,b=160:8f#5,8f#5,8f#5,8d5,8p,8b,8p,8e5,8p,8e5,8p,8e5,8g#5,8g#5,8a5,8b5,8a5,8a5,8a5,8e5,8p,8d5,8p,8f#5,8p,8f#5,8p,8f#5,8e5,8e5,8f#5,8e5,8f#5,8f#5,8f#5,8d5,8p,8b,8p,8e5,8p,8e5,8p,8e5,8g#5,8g#5,8a5,8b5,8a5,8a5,8a5,8e5,8p,8d5,8p,8f#5,8p,8f#5,8p,8f#5,8e5,8e5";
//char weRock[] = "we-rock:d=4,o=6,b=80:16a5,16a5,16c6,16e.6,8d.6,4a5,16a5,16a5,16g5,16c.6,4a.5,16a5,16a5,16c6,16e.6,8d.6,4a6,16g6,16g6,16e6,16c.6,2a6,16c7,16a#6,16a6,16g6,16f6,16e6,16d6,16e6,16c6,4d6,16d6,16c7,16a#6,16g#6,16g6,16e6,16d6,16c6,16d6,16e.6,16d6,16c.6,8d6,4a6,16g6,16g6,16e6,16c.6,2d6,32f.5,8f#5,32f.5,32f#.5,16g#5,16f#5,4f.5,32f.5,16f#.5,32f.5,32f#.5,16f5,16d#5,4a#.5,32f.5,8f#5,32f.5,32f#.5,16g#5,16f#5,4f.5,32a5,16a#.5,32a.5,32a#.5,16c#6,16a#5,16g#.5,16f.5,4d5,8a#6,16g#6,16f#6,16f6,16d#6,16f6,16a#5,4d#6,8d#6,16c#6,16b6,16a6,16g#6,16f#6,16d#6,16f.6,16d#6,16c#.6,8d#6,4a#6,16g#6,16g#6,16f6,16c#.6,4d#.6,16g#6,16g#6,16f6,16g#6,16c#6,4d#.6,32f.5,8f#.5,32a#.5,2b5,16c#.5,16c#.5,16g#.5,16d#.5,16a#.5";
char indi[] = "IndianaJones:d=4,o=5,b=250:e,8p,8f,8g,8p,2c.6,8p.,d,8p,8e,1f,p.,g,8p,8a,8b,8p,2f.6,p,a,8p,8b,2c6,2d6,2e6,e,8p,8f,8g,8p,1c6,p,d6,8p,8e6,1f6,g,8p,8g,e.6,8p,d6,8p,8g,e.6,8p,d6,8p,8g,f.6,8p,e6,8p,8d6,1c6";
char cal2pac[] = "2Pac - California Love : d=4,o=5,b=90:8g.6,16f6,8c#6,8d6,8f6,d.6,2p,8g,8a#,8d6,8d6,p,8p,8g,8g,16g,8f.,8g,2p,8g,16a#,16g,8d6,2d6,16g,8g.,g,2p,8g,8a#,8d6,2d.6,16g,16g,8g,8f,8g,p,8g,8c6,8c6,8a#,8a,g,p,8g,c6,8a#,8a,2g,";
//char tetris[] = "Tetris:d=4,o=6,b=80:8f7,16c7,16c_7,16d_7,32f7,32d_7,16c_7,16c7,8a_,16a_,16c_7,8f7,16d_7,16c_7,8c.7,16c_7,8d_7,8f7,8c_7,8a_,a_,8d_7,16f_7,8a_7,16g_7,16f_7,8f.7,16c_7,8f7,16d_7,16c_7,8c7,16c7,16c_7,8d_7,8f7,8c_7,8a_,8a_";
//char melody_tones[] = "tetris:d=4,o=5,b=63:8c,32p,8d,32p,8e,32p,8f,32p,8g,32p,8a,32p,8b,32p\0";
char dre[] = "TheNextE:d=4,o=5,b=90:8e,8p,8b,8p,16b,16p,16a,16p,8b,8p,16a,16p,16g,16p,8a,8p,16a,16p,16g,16p,16e,16p,16g,16p,8e,8p,8b,8p,16b,16p,16a,16p,8b,8p,16a,16p,16g,16p,8a,8p,16a,16p,16g,16p,16e,16p,16g";
char doom[] = "Doom:d=32,o=5,b=56:f,f,f6,f,f,d#6,f,f,c#6,f,f,b,f,f,c6,c#6,f,f,f6,f,f,d#6,f,f,c#6,f,f,8b.,f,f,f6,f,f,d#6,f,f,c#6,f,f,b,f,f,c6,c#6,f,f,f6,f,f,d#6,f,f,c#6,f,f,8b.,a#,a#,a#6,a#,a#,g#6,a#,a#,f#6,a#,a#,e6,a#,a#,f6,f#6,a#,a#,a#6,a#,a#,g#6,a#,a#,f#6,a#,a#,8e6";
char drwho[] = "Dr Who:d=4,o=6,b=80:b.5,8g5,16b.5,8a.5,32g.5,32f#5,g.5,e,32d,32c,8d,8g5,8e.,32d,32c,8d,8b5,2a5,32g5,32f#5,2g5";
char startreck[] = "we-rock:d=4,o=6,b=225:2b.4,4e.5,1a5,4g#.5,4e.5,4c#.5,4f#.5,2b.5,4b.5,2d#.6,";
char starwars[] = "SWEnd:d=4,o=5,b=225:2c,1f,2g.,8g#,8a#,1g#,2c.,c,2f.,g,g#,c,8g#.,8c.,8c6,1a#.,2c,2f.,g,g#.,8f,c.6,8g#,1f6,2f,8g#.,8g.,8f,2c6,8c.6,8g#.,8f,2c,8c.,8c.,8c,2f,8f.,8f.,8f,2f";
char pacman[] = "Pacman:d=16,o=6,b=140:b5,b,f#,d#,8b,8d#,c,c7,g,f,8c7,8e,b5,b,f#,d#,8b,8d#,32d#,32e,f,32f,32f#,g,32g,32g#,a,8b";



// ===================================================================================
RTTTL rtttl = RTTTL(pinBuzzer);

bool isWaiting=false;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(ledR, OUTPUT);     
  pinMode(ledY, OUTPUT);     
  pinMode(ledG, OUTPUT);     
  pinMode(ledB, OUTPUT);     
  
  Serial.begin(115200);
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("RemoteLeds :");
  rtttl.playMelody(starwars);
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

  if(ledStatus & 0x08)
    digitalWrite(ledB, HIGH);   // turn the LED on (HIGH is the voltage level)
  else
    digitalWrite(ledB, LOW);   // turn the LED on (HIGH is the voltage level)
}


// the loop routine runs over and over again forever:
void loop() {
  showLed();
  rtttl.tick();
  RTTTL_NoteInfos* note = &rtttl.m_currentNote;
 // printint("address ",(int)&note);
 // Serial.println("");

  if(note->info==RTTTL_INFO_EOM)
      ledStatus = 0;
  else if(note->note<3)
      ledStatus = 1;
  else if (note->note<6)
      ledStatus = 2;
  else if (note->note<9)
      ledStatus = 4;
  else if (note->note<12)
      ledStatus = 8;
  else
      ledStatus = 0;

  if (irrecv.decode(&results)) {
//    Serial.println("Got something");
    Serial.println(results.value,HEX);
    switch(results.value)
    {
      case 0xFF02FD: // pause resume      
        rtttl.pauseMelody();
        break;
      case 0xFF6897: //0
//        rtttl.playMelody(final_count_down);
        rtttl.playMelody(pacman);
        break;
      case 0xFF30CF: //1
        //rtttl.playMelody(takeOnMe);
        rtttl.playMelody(cal2pac);
        
        break;
      case 0xFF18E7: //2
        rtttl.playMelody(dre);
        break;
      case 0xFF7A85: //3
        rtttl.playMelody(indi);
        break;
      case 0xFF10EF: //4
      //  rtttl.playMelody(cal2pac);
      rtttl.playMelody(doom);      
        break;
      case 0xFF38C7: //5
        rtttl.playMelody(drwho);
      // rtttl.playMelody(tetris);
        break;
      case 0xFF5AA5: //6
        rtttl.playMelody(startreck);
        break;
      case 0xFF42BD: //7
        rtttl.playMelody(starwars);
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
