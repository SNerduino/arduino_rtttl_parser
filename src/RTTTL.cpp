# include "RTTTL.h"

#define _CHECK_EOS_(strNote) if(strNote=='\0') {\
                                return RTTTL_ERR_INCOMPLETE_NOTE;\
                              }

RTTTL::RTTTL(int buzzerPin)
{
  m_buzzer_pin  = buzzerPin;
  pinMode(m_buzzer_pin, OUTPUT);
    
}
void printint(char * title, int i)
{
  char str[20];
  Serial.print(title);
  itoa(i,str,10);
  Serial.print(str);
}


int RTTTL::playMelody(char *melody, int _loop)
{
  int index=0;
  int totalstrLen;
  
  m_loop        = _loop; 
  m_melody      = melody;
  melody_title  = melody;
  totalstrLen= strlen(m_melody);
#ifdef RTTTL_DEBUG
  Serial.println(m_melody);
  printint("totalstrLen = ", totalstrLen);
  Serial.println("");
#endif
  // Find melody Name
  while(m_melody[index]!=':' && index<totalstrLen)
  {
    index++;
  }
  if(index==totalstrLen)return -1;
  memcpy(m_title,m_melody,index);
  m_title[index]='\0';
  Serial.println(m_title);
  index++;
  char *melodyParams = &m_melody[index];
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
                return-1;
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
                return -1;
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
                return -1;
            }
        }
    }
    
    index++;
  }  

  
  if(index<totalstrLen)
  {
     m_notes=&m_melody[index+1];
     m_noteLen=strlen(m_notes);
#ifdef RTTTL_DEBUG
     printint("m_noteLen = ",m_noteLen);
    Serial.println("");
#endif
  }  
  m_full_time = 3*(60000/m_tempo);
#ifdef RTTTL_DEBUG
  printint("Infos : b=",m_tempo);
  printint(", o=",m_default_octave);
  printint(", d=",m_default_duration);
  Serial.println("");
#endif
  m_notePointer = 0;
  m_prev_time=millis();
  return 0;
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
        (*val)  *=10;
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
            if(isSharp!=1)(*toneID)=0; else (*toneID)=1;
            break;
        case 'd':
            if(isSharp!=1)(*toneID)=2; else (*toneID)=3;
            break;
        case 'e':
            (*toneID)=4; 
            break;
        case 'f':
            if(isSharp!=1)(*toneID)=5; else (*toneID)=6;
            break;
        case 'g':
            if(isSharp!=1)(*toneID)=7; else (*toneID)=8;
            break;
        case 'a':
            if(isSharp!=1)(*toneID)=9; else (*toneID)=10;
            break;
        case 'b':
            (*toneID)=11;
            break;
        case 'p':
            (*toneID)=12;
            break;
    }
    if((*toneID)>=0)
        if(isSharp>=1) return 2;
        else return 1;
   else return -1;
    
}

int RTTTL::parseNote(char* note)
{
      // C #C D #D E F #F G #G A #A B 
#ifdef RTTTL_DEBUG
      Serial.println(note);
#endif
      int base_tones[] = { 261, 277, 293, 311, 329.63, 349, 370, 392, 415, 440, 466, 493};
      int index             = 0;
      m_currentNote.octave            = m_default_octave;
      m_currentNote.duration          = m_default_duration;
      m_currentNote.is_dotted         = false;
      noTone(m_buzzer_pin);
      _CHECK_EOS_(note[index]);
      // GET DURATION
      int val;
      int res = getUnsignedInt(&note[index], &val);
      if(res>=0) {m_currentNote.duration = val; index+=res;}
      
      _CHECK_EOS_(note[index]);
      res = getTone(&note[index], &val);
      if(res>=0) {m_currentNote.note = val; index+=res;
#ifdef RTTTL_DEBUG
      printint("get tone offset :",res); Serial.println("");
#endif   
      }
      if(note[index]=='.'){m_currentNote.is_dotted=true;index++;}
      if(note[index]!='\0')
      {
        res = getUnsignedInt(&note[index], &val);
        if(res>=0) {m_currentNote.octave = val; index+=res;}
      }
      // Play Note
      if(m_currentNote.note!=12)
      {
        int finalTone = (base_tones[m_currentNote.note])*(m_currentNote.octave/4);
        tone(m_buzzer_pin, finalTone);
      }
      // Set duration 
      m_interval = (m_full_time/m_currentNote.duration)  ; 
      if(m_currentNote.is_dotted)
      {
        m_interval = m_interval/2+m_interval ; 
      }
      m_prev_time =  millis(); 
#ifdef RTTTL_DEBUG
      printint("t=",m_currentNote.note);
      printint(", o=",m_currentNote.octave);
      printint(", d=",duration);
      Serial.println("");
#endif
      return index;
}


RTTTL_NoteInfos  *RTTTL::tick()
{
  if ((millis()-m_prev_time >= m_interval) ) {
    noTone(m_buzzer_pin);
    // Detect pause
    if(m_pause) 
    {
      m_currentNote.info = RTTTL_INFO_NONE;
      return &m_currentNote;
    }
    if(m_notePointer < m_noteLen)
    {
      m_prev_time = millis();
      char strNote[7];
      int index=0;
      while((m_notes[m_notePointer]==',' || m_notes[m_notePointer]==' ') && m_notePointer<m_noteLen)
        m_notePointer++;
      if(m_notePointer==m_noteLen) return NULL; // Problem !!!  
      
      while(m_notes[m_notePointer+index]!=',' && m_notePointer+index<m_noteLen && index<6)
      {
        strNote[index]=m_notes[m_notePointer+index];
        index ++;
      }
      strNote[index]='\0';
      int res = parseNote(strNote);
      if(res>0)
      {
        m_notePointer += index+1;
      } 
      if(m_notePointer >= m_noteLen)
      {
#ifdef RTTTL_DEBUG
        Serial.println("Finished");
#endif
        if(m_loop)
           m_notePointer=0;
      }
      m_currentNote.info = RTTTL_INFO_NONE;
      return &m_currentNote;
    }
    m_currentNote.info = RTTTL_INFO_EOM;
    return &m_currentNote;
  }
}

void RTTTL::pauseMelody()
{
  m_pause=true;
}

void RTTTL::continueMelody()
{
  m_pause = false;
}

