// =========================================
/**
 * \file RTTTL.h
 * \brief A library for non blocking RTTTL melody parsing and playing for Arduino applications 
 * \author Saifeddine ALOUI
 * \version 1.0
 * \date 28 october 2017
 *
 * Fully featured, non blocking RTTTL parser and player for Arduino boards.
 *
 */
// =========================================

#ifndef RTTTL_H
#define RTTTL_H

#include "Arduino.h"

// =========================================
// Configurations
// =========================================
/// A debug macro. Uncomment if you want to debug the library
//#define RTTTL_DEBUG
// =========================================
// Defines
// =========================================
#define RTTTL_ERR_INCOMPLETE_NOTE (-1)

// / \brief 
/**
 * @defgroup Note information list
 *
 * @{
 */

/** No information available. */
#define RTTTL_INFO_NONE            (0)
/** Generic Parsing Error. */
#define RTTTL_INFO_PARSING_ERR     (1)
/** Playing is paused by the user. */
#define RTTTL_INFO_PAUSED          (2)
/** End Of Melody detected. */
#define RTTTL_INFO_EOM             (3)
/** Incomplete sequence was detected. */
#define RTTTL_INFO_INCOMPLETE_SEQ  (4)
/** @} */

///\breaf Note parsing information
class RTTTL_NoteInfos{
public :
  int duration; 	/*!< The duration of the note */
  int note;       	/*!< The note (0 to 11 for regular notes C C# D D# E F F# G G# A A# B) and 12 for P (silence) */
  int octave;    	/*!< The note octave 4 to 7 */
  int is_dotted;	/*!< Tells weather a half time should be added to the duration */
  int info;         /*!< Information about the current status of the parsing */
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
/// \brief The main RTTTL class
/*! \class RTTTL
* \brief  The main RTTTL class
*
*  Fully featured RTTTL parser and player class
*/
class RTTTL
{
private:
    int      m_loop=false;
    char *   m_melody=0;
    char *   m_notes=0;
    char     m_title[26];

    int         m_melody_len=0;
	/**
	 * @defgroup Default stuff
	 *
	 * @{
	 */
	 /// The default duration of a note 
    int         m_default_duration=4;
	 /// The default octave of a note 
    int         m_default_octave=6; 
	 /// The melody tempo 
    int         m_tempo=64;
	/** @} */

	/// Thre buzzer pin
    int         m_buzzer_pin=0;
    
	
    int         m_notePointer=0;
    int         m_noteLen=0;
    
    unsigned long m_full_time=2500;
    unsigned long m_prev_time;
    unsigned long m_interval;
    

   
    int getUnsignedInt(char *str, int *val);
    int getTone(char *str, int *toneID);
    int parseNote(char *note);
	
	/// Is the song paused or is it playing
    int        m_pause=false;
public :
	/// This gives information about the current Note being played as well as the status of the last note parsing
	RTTTL_NoteInfos  m_currentNote;   
	/// The title of the current melody
    char *   melody_title;
	/**
     * \brief The constructor of the class
	 * \param buzzerPin the pin id that was used for the buzzer
	*/
    RTTTL(int buzzerPin);
    /**
	 * \brief Start playing a melody
	 * \param melody A null terminated string containing the RTTTL code
	 * \param _loop Tells weather the melody should be indefinitely played. Default = false.
	 */
    int playMelody(char *melody, int _loop=false, bool is_blocking=false);
    /**
	 * \brief Toggles pause the currently played melody (pause or resume) 
	 */
    void pauseMelody();
    /**
	 * \brief Runs the RTTTL parser iteration (check if next note should be parsed. If yes, parse the note and set time for next note parsing. If nothing to be done, just return). This is a non blocking function
	 */
    void tick();
};

#endif //RTTTL_H

