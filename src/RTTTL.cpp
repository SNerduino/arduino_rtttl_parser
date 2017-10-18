# include "rtttl_lib.h"

#define _CHECK_EOS_(strNote) if(strNote=='\0') return RTTTL_ERR_INCOMPLETE_NOTE;

RTTTL::RTTTL(char * melody, int buzzerPin)
{
  m_melody      = melody;
  melody_title   = melody;
  m_buzzerPin  = buzzerPin;
  pinMode(m_buzzerPin, OUTPUT);
    
  int index=0;
  int totalstrLen= strlen(m_melody);
  
  // Find melody Name
  while(m_melody[index]!=':' && index<totalstrLen)
  {
    index++;
  }
  if(index==totalstrLen)return;
  m_melody[index]='\0';
  
  char *melodyParams = &m_melody[index+1];
  // Extract parameters
  while(m_melody[index]!=':' && index<totalstrLen)
  {
    if(m_melody[index]=='d')
    {
        index++;
        if(m_melody[index] == '=')
        {
            index++;
            int val;
            int size = getUnsignedInt(&m_melody[index], &val);
            if(size>0)
            {
                m_default_duration = val;
                index += size;
            }
            else
            {
                return;
            }
        }
    }
    if(m_melody[index]=='o')
    {
        index++;
        if(m_melody[index] == '=')
        {
            index++;
            int val;
            int size = getUnsignedInt(&m_melody[index], &val);
            if(size>0)
            {
                m_default_octave = val;
            }
            else
            {
                return;
            }
        }
    }
    if(m_melody[index]=='b')
    {
        index++;
        if(m_melody[index] == '=')
        {
            index++;
            int val;
            int size = getUnsignedInt(&m_melody[index], &val);
            if(size>0)
            {
                m_tempo = val;
            }
            else
            {
                return;
            }
        }
    }
    index++;
  }  

  
  if(index<totalstrLen)
  {
     m_notes=&m_melody[index+1];
  }  
  m_notePointer = 0;
  prev=millis();
}

int RTTTL::getUnsignedInt(char *str, int *val)
{
            char strInt[4];
            (*val)              = 0;
            int index        = 0;
            char _str__id = str[index]-'0'; 
            if(_str__id < 0 || _str__id > 9) return -1;
            while(_str__id >= 0 && _str__id <= 9)
            {
                (*val)  * =10;
                (*val)  += _str__id;
                index++;
                _str__id = str[index] - '0'; 
            }
            return index;
}

int RTTTL::getTone(char *str, int *toneID)
{
      // C C# D D# E F F# G G# A A# B
    int isSharp=0;
    (*toneID) =-1;
    if(str[1]=='#') isSharp =1;
    switch(str[0])
    {
        case 'c':
            if(isSharp==0)(*toneID)=0; else (*toneID)=1;
            break;
        case 'd':
            if(isSharp==0)(*toneID)=2; else (*toneID)=3;
            break;
        case 'e':
            (*toneID)=4; ;
            break;
        case 'f':
            if(isSharp==0)(*toneID)=5; else (*toneID)=6;
            break;
        case 'g':
            if(isSharp==0)(*toneID)=7; else (*toneID)=8;
            break;
        case 'a':
            if(isSharp==0)(*toneID)=9; else (*toneID)=10;
            break;
        case 'b':
            (*toneID)=11;
            break;
        case 'p':
            (*toneID)=12;
            break;
    }
    if((*toneID)>=0)
        if(isSharp==1) return 2;
        else return 1;
   else return -1;
    
}

int parseNote(char* note)
{
      // C #C D #D E F #F G #G A #A B
      int base_tones[] = { 523, 554, 587, 622, 660, 698, 740, 784, 831, 880, 932, 988};
      int index               = 0;
      int octave            = mdefault_octave;
      int duration         = m_default_duration;
      int tone_id           = 0;
      int index              = 0;
      noTone(pinBuzzer);
      _CHECK_EOS_(note[index]);
      int val;
      int res = getUnsignedInt(note[index], &val);
      if(res>=0) {duration = val; index+=res;}
      _CHECK_EOS_(note[index]);
      res = getTone(note[index], &val);
      if(res>=0) {tone_id = val; index+=res;}
      _CHECK_EOS_(note[index]);
      res = getUnsignedInt(note[index], &val);
      if(res>=0) {octave = val; index+=res;}
      // Play Note
      int finalTone = (base_tones[tone_id]/5)*(octave);
      tone(m_buzzerPin, finalTone);
      // Set duration 
      interval = full_time/duration; 
      previous =  millis(); 
      return index;
}


void RTTTL::tick()
{
  if ((millis()-previous >= interval) ) {
    previous = millis();
    int res = parseNote(m_notes[m_notePointer]);
    if(res>0)
    {
        m_notePointer += res;
    } 
  }
}
