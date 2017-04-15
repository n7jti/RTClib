// door

#include <arduino.h>
#include "door.h"

#define MS_PER_MIN 60000

Door::Door()
  : _pin(0)
  , _state(DOOR_GREEN)
{
  
}

void Door::begin(int pin)
{
  // the door is open when the DOOR_PIN is HIGH. 
  _pin = pin;
  pinMode(_pin, INPUT_PULLUP);
}

DOOR_STATE Door::run()
{
  unsigned int now = millis();
  unsigned int elapsed = now - _stateStartMs;
  DOOR_STATE startState = _state;
  
  // The magnetic switch connects the pin to ground.
  // The magnetic switch is normally open, but is pulled closed by the magnet.
  // The pin is pulled-up and reads high when the switch is open 
  // We want our state variables to be HIGH when the door is open and LOW when closed. 
  // The pin reads high when the door is open. 
  int doorIn = digitalRead(_pin);

  switch(_state)
  {
    case DOOR_RED:
      if (doorIn == LOW) // closed
      {
        _state = DOOR_GREEN;
      }
    break;
    case DOOR_YELLOW:
      if (doorIn == LOW) // closed
      {
        _state = DOOR_GREEN;
      }
      else if (elapsed > MS_PER_MIN) // yellow for more than 1 minute
      {
         _state = DOOR_RED;
      }
    break;
    case DOOR_GREEN:
      if (doorIn == HIGH) // opened
      {
        _state = DOOR_YELLOW;
      }
    break;
  }

  // if we have changed state, then update the time the state started. 
  if (_state != startState)
  {
    _stateStartMs = now;
  }
  
  return _state;
}

DOOR_STATE Door::getState()
{
  return _state;
}

unsigned int Door::getStateElapsedMs()
{
  return millis() - _stateStartMs;
}

