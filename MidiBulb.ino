#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

const int bulbPinA = 4;
const int bulbPinB = 5;
const int bulbPinC = 6;
const int bulbPinD = 7;
const int statusLed = 13;

class ExpressionFlasher
{
    int m_value;
    int m_duration;
    int m_bulbPin;
    bool m_state;
    unsigned long m_previousMillis;
    unsigned long m_currentMillis;

    // Constructor
    public:
    ExpressionFlasher(int bulbPin)
    {
      m_bulbPin = bulbPin;
      m_value = 0;
      m_previousMillis = 0;
      m_state = false;
    }

    void Change(int value)
    {
      m_value = value;
      m_duration = random(5, 60) * (map(m_value, 10, 127, 100, 5));
    }

    void Update()
    {
      m_currentMillis = millis();
      if (m_value >= 127) {
        digitalWrite(m_bulbPin, HIGH);
      } else if (m_value > 20) {
        if (!m_state && m_currentMillis - m_previousMillis >= m_duration ) {
          digitalWrite(statusLed, HIGH);
          digitalWrite(m_bulbPin, HIGH);
          m_state = true;
          m_previousMillis = m_currentMillis;
        } else if (m_state && m_currentMillis - m_previousMillis >= m_duration) {
          digitalWrite(statusLed, LOW);
          digitalWrite(m_bulbPin, LOW);
          m_state = false;
          m_previousMillis = m_currentMillis;
        }
      } else if (m_state) {
        digitalWrite(statusLed, LOW);
        digitalWrite(m_bulbPin, LOW);
        m_state = false;
      }
    }
};

ExpressionFlasher flasherA(bulbPinA);
ExpressionFlasher flasherB(bulbPinB);
ExpressionFlasher flasherC(bulbPinC);
ExpressionFlasher flasherD(bulbPinD);

void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  switch ( pitch % 4 ) {
    case 0:
      digitalWrite(bulbPinA, HIGH);
      break;
    case 1:
      digitalWrite(bulbPinB, HIGH);
      break;
    case 2:
      digitalWrite(bulbPinC, HIGH);
      break;
    case 3:
      digitalWrite(bulbPinD, HIGH);
      break;
  }
  digitalWrite(statusLed, HIGH);
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
  switch ( pitch % 4 ) {
    case 0:
      digitalWrite(bulbPinA, LOW);
      break;
    case 1:
      digitalWrite(bulbPinB, LOW);
      break;
    case 2:
      digitalWrite(bulbPinC, LOW);
      break;
    case 3:
      digitalWrite(bulbPinD, LOW);
      break;
  }
  digitalWrite(statusLed, LOW);
}

void handleControlChange(byte channel, byte number, byte value)
{
  if ( number == 9 ) {
    flasherA.Change(value);
    flasherB.Change(value);
    flasherC.Change(value);
    flasherD.Change(value);
  }
}

void setup()
{
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.setHandleControlChange(handleControlChange);
  MIDI.begin(MIDI_CHANNEL_OMNI);

  pinMode(bulbPinA, OUTPUT);
  pinMode(bulbPinB, OUTPUT);
  pinMode(bulbPinC, OUTPUT);
  pinMode(bulbPinD, OUTPUT);
}

void loop()
{
  MIDI.read();
  flasherA.Update();
  flasherB.Update();
  flasherC.Update();
  flasherD.Update();
}
