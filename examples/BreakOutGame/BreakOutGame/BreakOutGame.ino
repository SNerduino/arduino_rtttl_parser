/**
 * \file BreakOutGame.ino
 * \brief A demonstration how to use RTTTL library for sound management in games
 * \author Saifeddine ALOUI
 * \version 1.0
 * \date 28 october 2017
 *
 * Simple breakout game using a 8X8 Matrix led driven by MAX7219 driver and using the IRemote library for controlling the motion.
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
char ping[]="ping:d=4,o=5,b=100:16e6";
char pong[]="pong:d=4,o=5,b=100:16c4";
char wall[]="wall:d=4,o=5,b=100:16f4";
char fool[]="smbdeath:d=4,o=5,b=90:32c6,32c6,32c6,8p,16b,16f6,16p,16f6,16f.6,16e.6,16d6,16c6,16p,16e,16p,16c";
char gameTheme[]="korobyeyniki:d=4,o=5,b=160:e6,8b,8c6,8d6,16e6,16d6,8c6,8b,a,8a,8c6,e6,8d6,8c6,b,8b,8c6,d6,e6,c6,a,2a,8p,d6,8f6,a6,8g6,8f6,e6,8e6,8c6,e6,8d6,8c6,b,8b,8c6,d6,e6,c6,a,a";
// ===================================================================================

// Instantiate the RTTTL object
RTTTL rtttl = RTTTL(pinBuzzer);
char bricks[4];
int racketPos=3;
int ballPos_x=3;
int ballPos_y=6;
int ballSpd_x=0;
int ballSpd_y=0;
// ===================================================
// The setup routine runs once when you press reset:
// ===================================================
void initScene()
{
  // First 4 lines full
  for(int i=0;i<4;i++)
  {
    bricks[i] = 0xFF;
  }
}

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
  Serial.println("BreakOut Game :");
  initScene();
  //
  rtttl.playMelody(gameTheme);
}

// ===================================================
// A function to update led matrix display
// ===================================================
void updateDisplay()
{
  // Show bricks
  for(int i=0;i<4;i++)
    lc.setRow(0, i, bricks[i]);
  // Empty space
  for(int i=4;i<7;i++)
  {
    lc.setRow(0,i, 0x00);
  }
    
  // The ball
    lc.setLed(0,ballPos_y, ballPos_x, true);    
    
  // The racket
    lc.setRow(0,7, B111<<(7-racketPos-1));
}

// ===================================================
// the loop routine runs over and over again forever:
// ===================================================
long lastMotionTime = 0;
long gameSpeedTime = 300;
bool isPlaying=false;
bool playingMelody=true;
int lives =5;
void reset()
{
    lives=5;
    for(int i=0;i<4;i++)
      bricks[i]=0xFF; // Start Over Again
    rtttl.playMelody(gameTheme);
    ballSpd_y = 0;
    ballSpd_x = 0;
    ballPos_x = 4;
    ballPos_y = 6;
    racketPos = 4;
}
void loop() {
  // Call the rtttl parser to update playing status
  rtttl.tick();
  // Recover last parsing note information
  RTTTL_NoteInfos* note = &rtttl.m_currentNote;
  // Detect the end of the game themo song to start the game
  if(playingMelody && note->info==RTTTL_INFO_EOM)
    playingMelody=false;
  // Show the scene
  updateDisplay();

  long time_ = millis();
  // Game can only start once the game theme melidy is finished
  if(!playingMelody)
  {
    if(time_ - lastMotionTime >=gameSpeedTime)
    {
      lastMotionTime=time_;
      if(isPlaying)
      {
        // Move the ball
        ballPos_x = ballPos_x +ballSpd_x;
        ballPos_y = ballPos_y +ballSpd_y;
    
        // Detect contact with wall
        if(ballPos_y<=0) // Up wall
        {
           ballSpd_y = 1;
           rtttl.playMelody(wall);
        }
        if(ballPos_x<=0) // Left wall
        {
           ballSpd_x = 1;
           rtttl.playMelody(wall);
        }
        if(ballPos_x>=7) // Right wall
        {
           ballSpd_x = -1;
           rtttl.playMelody(wall);
        }
        if(ballPos_y>=7) // Down, wall Lost !!
        {
           ballSpd_y = 0;
           ballSpd_x = 0;
           ballPos_x = 4;
           ballPos_y = 6;
           racketPos = 4;
           rtttl.playMelody(fool);
           isPlaying=false;
           lives--;
           if(lives == 0)
           {
            reset();
           }
           return;
        }
        if(ballPos_y==6) // Contact with racket
        {
          if(ballPos_x==racketPos-2 && ballSpd_x==1)
          {
            ballSpd_x = -1;
            ballSpd_y = -1;
            // Play ping
            rtttl.playMelody(ping);
          }
          if(ballPos_x==racketPos-1)
          {
            ballSpd_y = -1;
            if(ballSpd_x == 0) 
              ballSpd_x = -1;
            // Play ping
            rtttl.playMelody(ping);
          }
    
          if(ballPos_x==racketPos)
          {
            ballSpd_y = -1;
            // Play ping
           rtttl.playMelody(ping);
        }
    
          if(ballPos_x==racketPos+1)
          {
            ballSpd_y = -1;
            if(ballSpd_x == 0) 
              ballSpd_x = 1;
            // Play ping
            rtttl.playMelody(ping);
          }
          if(ballPos_x==racketPos+2 && ballSpd_x==-1)
          {
            ballSpd_x = 1;
            ballSpd_y = -1;
            // Play ping
            rtttl.playMelody(ping);
          }
        }
        if(ballPos_x>=7) // Right wall
        {
           ballSpd_x = -1;
        }
        // Detect contact with brick
        if(ballPos_y<4) // We are in brick zone
        {
            // Test weather a contact with a brick occured
            if(bricks[ballPos_y] & 1<<(7-ballPos_x))
            {
              bricks[ballPos_y] &= ~(1<<(7-ballPos_x));
              ballSpd_y=1;
              // Play pong
              rtttl.playMelody(pong);
            }
            // Test weather there is no more bricks
            bool isAllDone=true;
            for(int i=0;i<4;i++)
              if(bricks[i]!=0)
              {
                isAllDone=false;
                break;
              }
           if(isAllDone)
           {
             rtttl.playMelody(fool);
             isPlaying=false;
             lives=5;
             if(gameSpeedTime>50)
             {
               gameSpeedTime-=50; // Harder :) Speed gets faster
               for(int i=0;i<4;i++)
                 bricks[i]=0xFF; // Start Over Again
             }
             else
             {
             // Feel free to add stuff when all levels are completed  
              
             }
           }
        }
      }
    }
    
    // Check for remote control input  
    if (irrecv.decode(&results)) {
      Serial.println(results.value,HEX);
      switch(results.value)
      {
        case 0xFF02FD: // pause resume      
          rtttl.pauseMelody();
          break;
        case 0xFF6897: //0
          break;
        case 0xFF30CF: //1
          break;
        case 0xFF18E7: //2
          break;
        case 0xFF7A85: //3
          break;
        case 0xFF10EF: //4
          break;
        case 0xFF38C7: //5
          break;
        case 0xFF5AA5: //6
          break;
        case 0xFF42BD: //7
        if(racketPos>1)
        {
           racketPos--;
           if(!isPlaying)
           {
              ballPos_x--;            
           }
        }
          break;
        case 0xFF4AB5: //8
          rtttl.playMelody(ping);
          ballSpd_y=-1;
          isPlaying = true;
          break;
        case 0xFF52AD: //9
        if(racketPos<6)
        {
           racketPos++;
           if(!isPlaying)
           {
              ballPos_x++;            
           }
        }
        break;
        case 0xFFA25D: //On Off
        reset();
        break;
      }
      irrecv.resume(); // Receive the next value
    }
  }
}





