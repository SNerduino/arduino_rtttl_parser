// =========================================
// rtttl_lib
// Author : Saifeddine ALOUI
// =========================================
// =========================================
// Defines
// =========================================
#define RTTTL_ERR_INCOMPLETE_NOTE (-1)

// =========================================
// Function prototypes
// =========================================
class RTTTL
{
private:
    char *   m_melody=0;
    char *   m_notes=0;
    int         m_melody_len=0;
    int         m_default_duration=0;
    int         m_default_octave=0;
    int         m_tempo=0;
    int         m_buzzer_pin=0;
    
    int         m_notePointer=0;
    
    unsigned long m_full_time=600;
    unsigned long m_prev_time;
    unsigned long m_interval;

    
public :
    char *   melody_title;

    RTTTL(char * melody, int buzzerPin);
    int parseNote(char* note);
    void tick();
    int getUnsignedInt(char *str, int *val);
    int getTone(char *str, int *toneID);
};

