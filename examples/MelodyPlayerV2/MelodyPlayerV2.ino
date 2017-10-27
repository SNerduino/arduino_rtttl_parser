#include <LedControl.h> //  need the library
/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */
LedControl lc=LedControl(12,11,10,1); //
/*
  Sketch : MelodyPlayerV2
  Author : Saifeddine ALOUI
  This is a simple RTTTL melody player to illustrate how useful it is to use this library in a multitask invironment
  The player setup needs a ELEGOO remote control, MAX2719 based 8X8 Leds pannel + a buzzer + 300Ohms resistor + infrared receiver
  
  The player is remote controlled using the infrared remote control.
  The parsed notes are used to update the leds matrix.

  Depends on :
    wayoda's LedControl library https://github.com/wayoda/LedControl
    The Led Matrix as well as the buzzer use  

  3 activities are running in the same time : 
    - our RTTTL library
    - the led matrix playing
    - the remote control signal decoding
  
  This setup was tested on the arduino nano using the ATmega328 microcontroller 
  
*/
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

// =================== Imports ===============
#include <IRremote.h>
#include <RTTTL.h>
// ===========================================
// IR input pin
int RECV_PIN = 6;
IRrecv irrecv(RECV_PIN);
decode_results results;

// Buzzer output pin
const int pinBuzzer = 8;
// ===================================================================================
// Melodies
// ===================================================================================
// Select the melody you want or just place your own melody in a character array
char final_count_down[] = "FinalCountdown:d=4,o=5,b=125:16c#6,16b,c#6,f#,p.,16d6,16c#6,8d6,8c#6,b,p.,16d6,16c#6,d6,f#,p.,16b,16a,8b,8a,8g#,8b,a.,16c#6,16b,c#6,f#,p.,16d6,16c#6,8d6,8c#6,b,p.,16d6,16c#6,d6,f#,p.,16b,16a,8b,8a,8g#,8b,a.,16g#,16a,b.,16a,16b,8c#6,8b,8a,8g#,f#,d6,2c#.6,16c#6,16d6,16c#6,16b,1c#6,2p";
char takeOnMe[] = "TakeOnMe:d=4,o=4,b=160:8f#5,8f#5,8f#5,8d5,8p,8b,8p,8e5,8p,8e5,8p,8e5,8g#5,8g#5,8a5,8b5,8a5,8a5,8a5,8e5,8p,8d5,8p,8f#5,8p,8f#5,8p,8f#5,8e5,8e5,8f#5,8e5,8f#5,8f#5,8f#5,8d5,8p,8b,8p,8e5,8p,8e5,8p,8e5,8g#5,8g#5,8a5,8b5,8a5,8a5,8a5,8e5,8p,8d5,8p,8f#5,8p,8f#5,8p,8f#5,8e5,8e5";
//char weRock[] = "we-rock:d=4,o=6,b=80:16a5,16a5,16c6,16e.6,8d.6,4a5,16a5,16a5,16g5,16c.6,4a.5,16a5,16a5,16c6,16e.6,8d.6,4a6,16g6,16g6,16e6,16c.6,2a6,16c7,16a#6,16a6,16g6,16f6,16e6,16d6,16e6,16c6,4d6,16d6,16c7,16a#6,16g#6,16g6,16e6,16d6,16c6,16d6,16e.6,16d6,16c.6,8d6,4a6,16g6,16g6,16e6,16c.6,2d6,32f.5,8f#5,32f.5,32f#.5,16g#5,16f#5,4f.5,32f.5,16f#.5,32f.5,32f#.5,16f5,16d#5,4a#.5,32f.5,8f#5,32f.5,32f#.5,16g#5,16f#5,4f.5,32a5,16a#.5,32a.5,32a#.5,16c#6,16a#5,16g#.5,16f.5,4d5,8a#6,16g#6,16f#6,16f6,16d#6,16f6,16a#5,4d#6,8d#6,16c#6,16b6,16a6,16g#6,16f#6,16d#6,16f.6,16d#6,16c#.6,8d#6,4a#6,16g#6,16g#6,16f6,16c#.6,4d#.6,16g#6,16g#6,16f6,16g#6,16c#6,4d#.6,32f.5,8f#.5,32a#.5,2b5,16c#.5,16c#.5,16g#.5,16d#.5,16a#.5";
char indi[] = "IndianaJones:d=4,o=5,b=250:e,8p,8f,8g,8p,2c.6,8p.,d,8p,8e,1f,p.,g,8p,8a,8b,8p,2f.6,p,a,8p,8b,2c6,2d6,2e6,e,8p,8f,8g,8p,1c6,p,d6,8p,8e6,1f6,g,8p,8g,e.6,8p,d6,8p,8g,e.6,8p,d6,8p,8g,f.6,8p,e6,8p,8d6,1c6";
//char cal2pac[] = "2Pac - California Love : d=4,o=5,b=90:8g.6,16f6,8c#6,8d6,8f6,d.6,2p,8g,8a#,8d6,8d6,p,8p,8g,8g,16g,8f.,8g,2p,8g,16a#,16g,8d6,2d6,16g,8g.,g,2p,8g,8a#,8d6,2d.6,16g,16g,8g,8f,8g,p,8g,8c6,8c6,8a#,8a,g,p,8g,c6,8a#,8a,2g,";
//char tetris[] = "Tetris:d=4,o=6,b=80:8f7,16c7,16c_7,16d_7,32f7,32d_7,16c_7,16c7,8a_,16a_,16c_7,8f7,16d_7,16c_7,8c.7,16c_7,8d_7,8f7,8c_7,8a_,a_,8d_7,16f_7,8a_7,16g_7,16f_7,8f.7,16c_7,8f7,16d_7,16c_7,8c7,16c7,16c_7,8d_7,8f7,8c_7,8a_,8a_";
//char melody_tones[] = "tetris:d=4,o=5,b=63:8c,32p,8d,32p,8e,32p,8f,32p,8g,32p,8a,32p,8b,32p\0";
//char dre[] = "TheNextE:d=4,o=5,b=90:8e,8p,8b,8p,16b,16p,16a,16p,8b,8p,16a,16p,16g,16p,8a,8p,16a,16p,16g,16p,16e,16p,16g,16p,8e,8p,8b,8p,16b,16p,16a,16p,8b,8p,16a,16p,16g,16p,8a,8p,16a,16p,16g,16p,16e,16p,16g";
//char doom[] = "Doom:d=32,o=5,b=56:f,f,f6,f,f,d#6,f,f,c#6,f,f,b,f,f,c6,c#6,f,f,f6,f,f,d#6,f,f,c#6,f,f,8b.,f,f,f6,f,f,d#6,f,f,c#6,f,f,b,f,f,c6,c#6,f,f,f6,f,f,d#6,f,f,c#6,f,f,8b.,a#,a#,a#6,a#,a#,g#6,a#,a#,f#6,a#,a#,e6,a#,a#,f6,f#6,a#,a#,a#6,a#,a#,g#6,a#,a#,f#6,a#,a#,8e6";
//char drwho[] = "Dr Who:d=4,o=6,b=80:b.5,8g5,16b.5,8a.5,32g.5,32f#5,g.5,e,32d,32c,8d,8g5,8e.,32d,32c,8d,8b5,2a5,32g5,32f#5,2g5";
//char startreck[] = "we-rock:d=4,o=6,b=225:2b.4,4e.5,1a5,4g#.5,4e.5,4c#.5,4f#.5,2b.5,4b.5,2d#.6,";
//char starwars[] = "SWEnd:d=4,o=5,b=225:2c,1f,2g.,8g#,8a#,1g#,2c.,c,2f.,g,g#,c,8g#.,8c.,8c6,1a#.,2c,2f.,g,g#.,8f,c.6,8g#,1f6,2f,8g#.,8g.,8f,2c6,8c.6,8g#.,8f,2c,8c.,8c.,8c,2f,8f.,8f.,8f,2f";
//char pacman[] = "Pacman:d=16,o=6,b=140:b5,b,f#,d#,8b,8d#,c,c7,g,f,8c7,8e,b5,b,f#,d#,8b,8d#,32d#,32e,f,32f,32f#,g,32g,32g#,a,8b";
char mario[]="smb:d=4,o=5,b=100:16e6,16e6,32p,8e6,16c6,8e6,8g6,8p,8g,8p,8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,16p,8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16c7,16p,16c7,16c7,p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,8p,16d#6,8p,16d6,8p,16c6";
// ===================================================================================



// Instantiate the RTTTL object
RTTTL rtttl = RTTTL(pinBuzzer);

// ===================================================
// The setup routine runs once when you press reset:
// ===================================================
void setup() {                
  // initialize the digital pin as an output.
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,2);
  /* and clear the display */
  lc.clearDisplay(0);
  Serial.begin(115200);
  // Infrared receiver
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("RemoteLeds :");
  // The rtttl parser start playing 
  rtttl.playMelody(mario);
}
// ===================================================
// A function to update led matrix display
// ===================================================
char displayData[8];
char smiley[8]=
{B00111100,
 B01000010,
 B10010101,
 B10100001,
 B10100001,
 B10010101,
 B01000010,
 B00111100,
};

char noSmile= B10100101;

void updateDisplay()
{
  for(int i=0;i<8;i++)
    lc.setRow(0,i,displayData[i]);
}

// ===================================================
// the loop routine runs over and over again forever:
// ===================================================
long lastSmile = 0;
bool smileyStatus=false;
void loop() {
  // Call the rtttl parser to update playing status
  rtttl.tick();
  // Recover last parsing note information
  RTTTL_NoteInfos* note = &rtttl.m_currentNote;

  // Detect end of melody (can be used for looping)
  if(note->info==RTTTL_INFO_EOM)
  {
    long time_ = millis();
    if(time_-lastSmile>1000)
    {
      smileyStatus = !smileyStatus;
      lastSmile=time_;
    }
    for(int i=0;i<8;i++)
    {
       if(smileyStatus && (i==2 || i==5))
       {
               displayData[i]=noSmile;
       }
       else
       {
               displayData[i]=smiley[i];
       }
    }
  }
  else
  {
    for(int i=0;i<8;i++)
      displayData[i]=0x00;
    // Premare display data    
    if(note->note<2)
    {
        displayData[0]=0xff;
    }
    else if (note->note<4)
    {
        displayData[1]=0xff;
    }
    else if (note->note<6)
    {
        displayData[2]=0xff;
    }
    else if (note->note<8)
    {
        displayData[3]=0xff;
    }
    else if (note->note<10)
    {
        displayData[4]=0xff;
    }
    else if (note->note<12)
    {
        displayData[5]=0xff;
    }
  }
      

  updateDisplay();

  // Check for remote control input  
  if (irrecv.decode(&results)) {
    Serial.println(results.value,HEX);
    switch(results.value)
    {
      case 0xFF02FD: // pause resume      
        rtttl.pauseMelody();
        break;
      case 0xFF6897: //0
        rtttl.playMelody(final_count_down);
//        rtttl.playMelody(pacman);
        break;
      case 0xFF30CF: //1
        rtttl.playMelody(takeOnMe);
        //rtttl.playMelody(cal2pac);
        
        break;
      case 0xFF18E7: //2
        //rtttl.playMelody(dre);
        break;
      case 0xFF7A85: //3
        rtttl.playMelody(indi);
        break;
      case 0xFF10EF: //4
      //  rtttl.playMelody(cal2pac);
        rtttl.playMelody(mario);      
        break;
      case 0xFF38C7: //5
        rtttl.playMelody(mario);
      // rtttl.playMelody(tetris);
        break;
      case 0xFF5AA5: //6
      //  rtttl.playMelody(startreck);
        break;
      case 0xFF42BD: //7
      //  rtttl.playMelody(starwars);
        break;
      case 0xFF4AB5: //8
      //  rtttl.playMelody(indi);
        break;
      case 0xFF52AD: //9
      //  rtttl.playMelody(indi);
        break;
    }
    irrecv.resume(); // Receive the next value
  }
 
}





