#include <MIDI.h>
#include <SoftwareSerial.h>

SoftwareSerial softSerial(2,3);
MIDI_CREATE_INSTANCE(SoftwareSerial, softSerial, midiA);

const int bulbPinA = 4;
const int bulbPinB = 5;
const int bulbPinC = 6;
const int bulbPinD = 7;
const int statusLed = 13;

void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  switch( pitch % 4 ){
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
  }
  digitalWrite(statusLed, HIGH);
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
  switch( pitch % 4 ){
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
  }
  digitalWrite(statusLed, LOW);
}

void setup()
{
    midiA.setHandleNoteOn(handleNoteOn);
    midiA.setHandleNoteOff(handleNoteOff);
    midiA.begin(MIDI_CHANNEL_OMNI);
    
    pinMode(bulbPinA, OUTPUT);
    pinMode(bulbPinB, OUTPUT);
    pinMode(bulbPinC, OUTPUT);
    pinMode(bulbPinD, OUTPUT);
}

void loop()
{
    midiA.read();
}
