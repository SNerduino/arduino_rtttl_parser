// =========================================
// rtttl_lib
// Author : Saifeddine ALOUI
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

class RTTTL_NoteInfos{
public :
  int duration;
  int note;
  int octave;
  int is_dotted;
  int info;
};

// =========================================
// Function prototypes
// =========================================
void printint(char * title, int i);

// =========================================
// Main class definition
// =========================================
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

   
public :
   RTTTL_NoteInfos  m_currentNote;   
    char *   melody_title;

    RTTTL(int buzzerPin);
    void pauseMelody();
    int playMelody(char *melody, int _loop=false);
    int parseNote(char *note);
    void tick();
    int getUnsignedInt(char *str, int *val);
    int getTone(char *str, int *toneID);
};

#endif //RTTTL_H

