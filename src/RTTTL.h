// =========================================
// Library : RTTTL
// File    : RTTTL.h
// Author  : Saifeddine ALOUI
// Description :
// A library for non blocking RTTTL melody
// parsing and playing for Arduino applications 
// =========================================
#ifndef RTTTL_H
#define RTTTL_H

#include "Arduino.h"

// =========================================
// Configurations
// =========================================
#define RTTTL_DEBUG
// =========================================
// Defines
// =========================================
#define RTTTL_ERR_INCOMPLETE_NOTE (-1)

// Note information list
#define RTTTL_INFO_NONE            (0)
#define RTTTL_INFO_PARSING_ERR     (1)
#define RTTTL_INFO_PAUSED          (2)
#define RTTTL_INFO_EOM             (3)
#define RTTTL_INFO_INCOMPLETE_SEQ  (4)
/*
 * @breaf Note parsing information
 */
class RTTTL_NoteInfos{
public :
  int duration; 	// The duration of the note
  int note;       	// The note (0 to 11 for regular notes C C# D D# E F F# G G# A A# B) and 12 for P (silence)
  int octave;    	// The note octave 4 to 7
  int is_dotted;	// Tells weather a half time should be added to the duration
  int info;         // Information about the current status of the parsing
};

// =========================================
// Function prototypes
// =========================================
#ifdef RTTTL_DEBUG
/*
 * @brief prints an integer value to the Serial connection
 */
void printint(char * title, int i);
#endif
// =========================================
// Main class definition
// =========================================
/*
 * @brief The main RTTTL class
 */
class RTTTL
{
private:
    int      m_loop=false;
    char *   m_melody=0;
    char *   m_notes=0;
    char     m_title[26];

    int         m_melody_len=0;
    int         m_default_duration=4;
    int         m_default_octave=6; 
    int         m_tempo=64;
    int         m_buzzer_pin=0;
    
    int         m_notePointer=0;
    int         m_noteLen=0;
    
    unsigned long m_full_time=2500;
    unsigned long m_prev_time;
    unsigned long m_interval;
    
    int        m_pause=false;

   
    int getUnsignedInt(char *str, int *val);
    int getTone(char *str, int *toneID);
    int parseNote(char *note);
	
public :
	RTTTL_NoteInfos  m_currentNote;   
    char *   melody_title;

    /*
	 * @brief The constructor of the class
	 * @param buzzerPin the pin id that was used for the buzzer
	 */
    RTTTL(int buzzerPin);
    int playMelody(char *melody, int _loop=false);
    /*
	 * @brief Toggles pause the currently played melody (pause or resume) 
	 */
    void pauseMelody();
    /*
	 * @brief Runs the RTTTL parser iteration (check if next note should be parsed. If yes, parse the note and set time for next note parsing. If nothing to be done, just return). This is a non blocking function
	 */
    void tick();
};

#endif //RTTTL_H

