/**
 * \file MelodyPlayerV2.ino
 * \brief A remote controlled melody player based on RTTTL library
 * \author Saifeddine ALOUI
 * \version 2.0
 * \date 28 october 2017
 *
 * Plays melodies and displaying notes on a led matrix. A remote control enables changing the melody, pase and resume.
 *
 */
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
#include <RTTTL.h>
// ===========================================
// button pin
int btnPin = 6;

// Buzzer output pin
const int pinBuzzer = 8;
// ===================================================================================
// Melodies
// ===================================================================================
// Select the melody you want or just place your own melody in a character array
//char final_count_down[] = "FinalCountdown:d=4,o=5,b=125:16c#6,16b,c#6,f#,p.,16d6,16c#6,8d6,8c#6,b,p.,16d6,16c#6,d6,f#,p.,16b,16a,8b,8a,8g#,8b,a.,16c#6,16b,c#6,f#,p.,16d6,16c#6,8d6,8c#6,b,p.,16d6,16c#6,d6,f#,p.,16b,16a,8b,8a,8g#,8b,a.,16g#,16a,b.,16a,16b,8c#6,8b,8a,8g#,f#,d6,2c#.6,16c#6,16d6,16c#6,16b,1c#6,2p";
char takeOnMe[] = "TakeOnMe:d=4,o=4,b=160:8f#5,8f#5,8f#5,8d5,8p,8b,8p,8e5,8p,8e5,8p,8e5,8g#5,8g#5,8a5,8b5,8a5,8a5,8a5,8e5,8p,8d5,8p,8f#5,8p,8f#5,8p,8f#5,8e5,8e5,8f#5,8e5,8f#5,8f#5,8f#5,8d5,8p,8b,8p,8e5,8p,8e5,8p,8e5,8g#5,8g#5,8a5,8b5,8a5,8a5,8a5,8e5,8p,8d5,8p,8f#5,8p,8f#5,8p,8f#5,8e5,8e5";
//char weRock[] = "we-rock:d=4,o=6,b=80:16a5,16a5,16c6,16e.6,8d.6,4a5,16a5,16a5,16g5,16c.6,4a.5,16a5,16a5,16c6,16e.6,8d.6,4a6,16g6,16g6,16e6,16c.6,2a6,16c7,16a#6,16a6,16g6,16f6,16e6,16d6,16e6,16c6,4d6,16d6,16c7,16a#6,16g#6,16g6,16e6,16d6,16c6,16d6,16e.6,16d6,16c.6,8d6,4a6,16g6,16g6,16e6,16c.6,2d6,32f.5,8f#5,32f.5,32f#.5,16g#5,16f#5,4f.5,32f.5,16f#.5,32f.5,32f#.5,16f5,16d#5,4a#.5,32f.5,8f#5,32f.5,32f#.5,16g#5,16f#5,4f.5,32a5,16a#.5,32a.5,32a#.5,16c#6,16a#5,16g#.5,16f.5,4d5,8a#6,16g#6,16f#6,16f6,16d#6,16f6,16a#5,4d#6,8d#6,16c#6,16b6,16a6,16g#6,16f#6,16d#6,16f.6,16d#6,16c#.6,8d#6,4a#6,16g#6,16g#6,16f6,16c#.6,4d#.6,16g#6,16g#6,16f6,16g#6,16c#6,4d#.6,32f.5,8f#.5,32a#.5,2b5,16c#.5,16c#.5,16g#.5,16d#.5,16a#.5";
//char indi[] = "IndianaJones:d=4,o=5,b=250:e,8p,8f,8g,8p,2c.6,8p.,d,8p,8e,1f,p.,g,8p,8a,8b,8p,2f.6,p,a,8p,8b,2c6,2d6,2e6,e,8p,8f,8g,8p,1c6,p,d6,8p,8e6,1f6,g,8p,8g,e.6,8p,d6,8p,8g,e.6,8p,d6,8p,8g,f.6,8p,e6,8p,8d6,1c6";
//char tetris[] = "Tetris:d=4,o=6,b=80:8f7,16c7,16c_7,16d_7,32f7,32d_7,16c_7,16c7,8a_,16a_,16c_7,8f7,16d_7,16c_7,8c.7,16c_7,8d_7,8f7,8c_7,8a_,a_,8d_7,16f_7,8a_7,16g_7,16f_7,8f.7,16c_7,8f7,16d_7,16c_7,8c7,16c7,16c_7,8d_7,8f7,8c_7,8a_,8a_";
//char melody_tones[] = "tetris:d=4,o=5,b=63:8c,32p,8d,32p,8e,32p,8f,32p,8g,32p,8a,32p,8b,32p\0";
//char dre[] = "TheNextE:d=4,o=5,b=90:8e,8p,8b,8p,16b,16p,16a,16p,8b,8p,16a,16p,16g,16p,8a,8p,16a,16p,16g,16p,16e,16p,16g,16p,8e,8p,8b,8p,16b,16p,16a,16p,8b,8p,16a,16p,16g,16p,8a,8p,16a,16p,16g,16p,16e,16p,16g";
//char doom[] = "Doom:d=32,o=5,b=56:f,f,f6,f,f,d#6,f,f,c#6,f,f,b,f,f,c6,c#6,f,f,f6,f,f,d#6,f,f,c#6,f,f,8b.,f,f,f6,f,f,d#6,f,f,c#6,f,f,b,f,f,c6,c#6,f,f,f6,f,f,d#6,f,f,c#6,f,f,8b.,a#,a#,a#6,a#,a#,g#6,a#,a#,f#6,a#,a#,e6,a#,a#,f6,f#6,a#,a#,a#6,a#,a#,g#6,a#,a#,f#6,a#,a#,8e6";
//char drwho[] = "Dr Who:d=4,o=6,b=80:b.5,8g5,16b.5,8a.5,32g.5,32f#5,g.5,e,32d,32c,8d,8g5,8e.,32d,32c,8d,8b5,2a5,32g5,32f#5,2g5";
//char startreck[] = "we-rock:d=4,o=6,b=225:2b.4,4e.5,1a5,4g#.5,4e.5,4c#.5,4f#.5,2b.5,4b.5,2d#.6,";
char starwars[] = "SWEnd:d=4,o=5,b=225:2c,1f,2g.,8g#,8a#,1g#,2c.,c,2f.,g,g#,c,8g#.,8c.,8c6,1a#.,2c,2f.,g,g#.,8f,c.6,8g#,1f6,2f,8g#.,8g.,8f,2c6,8c.6,8g#.,8f,2c,8c.,8c.,8c,2f,8f.,8f.,8f,2f";
char pacman[] = "Pacman:d=16,o=6,b=140:b5,b,f#,d#,8b,8d#,c,c7,g,f,8c7,8e,b5,b,f#,d#,8b,8d#,32d#,32e,f,32f,32f#,g,32g,32g#,a,8b";
char blue[] ="Blue:d=8,o=5,b=140:a,a#,d,g,a#,c6,f,a,4a#,g,a#,d6,d#6,g,d6,c6,a#,d,g,a#,c6,f,a,4a#,g,a#,d6,d#6,g,d6,c6,a#,d,g,a#,c6,f,a,4a#,g,a#,d6,d#6,g,d6,c6,a#,d,g,a#,a,c,f,2g";
char titanic[] = "Titanic:d=8,o=6,b=125:e,f#,2g#,16f#,16g#,16f#,e,f#,2b,32c#7,32b,a,g#,4e,2c#,2b5,d#,f#,f#,2g#,16a,16g#,16f#,16e,4f#,2b,g#,b,2c#7,2b,2f#";
char MIP[] = "MImp:d=32,o=5,b=125:d6,d#6,d6,d#6,d6,d#6,d6,d#6,d6,d#6,d6,d#6,d6,d#6,d6,d#6,d6,d#6,d6,d#6,e6,f#6,d#6,8g.6,8g.,8g.,8a#,8c6,8g.,8g.,8f,8f#,8g.,8g.,8a#,8c6,8g.,8g.,8f,8f#,16a#6,16g6,2d6,16a#6,16g6,2c#6,16a#6,16g6,2c6,16a#,8c.6,4p,16a#,16g,2f#6,16a#,16g,2f6,16a#,16g,2e6,16d#6,8d.6";
//char justcalled[] = "Justcall:d=4,o=5,b=160:8c6,c6,2a,8c6,2b,8g,b,2c6,8p,8c6,c6,2a,8c6,b.,8a,g,a,2e,8p,8c6,c6,2a,8c6,2b,8g,b,2c6,8c6,c6,a.,8g,f,d,e,d,c,d,2c";
//char gump[]="Gump:d=4,o=7,b=125:8e6,8f6,8g6,g6,e6,g.6,8c,g6,e.6,8f6,8g6,8a6,a6,8f6,2a6,8p,8f6,8g6,8a6,a6,f6,a6,8d,b6,g.6,8e6,8f6,8g6,8g6,8c,2g6,8p,8a6,8b6,8c,c,a6,c.,8a6,c,a.6,8f6,8g6,8a6,a6,f6,2a6,8p,8f6,8g6,8a6,a6,f6,d.6,8e6,f6,d6,2c6";
//char mario[]="smb:d=4,o=5,b=100:16e6,16e6,32p,8e6,16c6,8e6,8g6,8p,8g,8p,8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,16p,8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16c7,16p,16c7,16c7,p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,8p,16d#6,8p,16d6,8p,16c6";
// ===================================================================================
char *song[]={starwars, takeOnMe, pacman, blue, titanic, MIP};
int nbMelodies = sizeof(song);
int melodyIndex=0;
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
  // Enable button
  pinMode(btnPin,INPUT);
  Serial.println("RemoteLeds :");
  // The rtttl parser start playing 
  rtttl.playMelody(song[melodyIndex++]);
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
long btnDebounceTime=0;
int previousVal=false;
void loop() {
  // Call the rtttl parser to update playing status
  rtttl.tick();
  // Recover last parsing note information
  RTTTL_NoteInfos* note = &rtttl.m_currentNote;
  long time_ = millis();

  // Detect end of melody (can be used for looping)
  if(note->info==RTTTL_INFO_EOM)
  {
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

  int val = digitalRead(btnPin);
  if(time_-btnDebounceTime>200) // 200ms debounce
  {
    //char str[3];
    //itoa(val,str,10);
    //Serial.print("Val =");
    //Serial.print(str);
    //Serial.print(" , previous Val =");
    //itoa(previousVal,str,10);
    //Serial.println(str);
      if(previousVal==1 and val==0) // Detect btn down event
      {
        Serial.println("Pressed");
        rtttl.playMelody(song[melodyIndex]);
        // Deterministic mode
        if(melodyIndex<nbMelodies-1)
          melodyIndex++;
        else
          melodyIndex=0;
      }
      btnDebounceTime=time_;
      previousVal=val;
  }
}





