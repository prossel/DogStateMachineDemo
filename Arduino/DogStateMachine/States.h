// This file declares and implements the State* classes, which are the children of State class
// and contains the logic of each state of our application.
//
// Pierre Rossel   2024-11-29  Initial version
//                 

#include "StateMachine.h"

// distance is a global variable declared in DogStateMachine.ino
extern int distance;

// speed is a global variable declared in DogStateMachine.ino
extern float speed;

// see doc/Dog State Machine.png for the state diagram

class StateIdle : public State
{
  void enter();
  State *loop();
  void exit(); 
};

class StateWatching : public State
{
  int limit = 180;
  void enter();
  State *loop();
  void exit(); 
};

class StateGreeting : public State
{
  void enter();
  State *loop();
  void exit(); 
};

class StateBarking : public State
{
  int enterDistance = 0;

  void enter();
  State *loop();
  void exit(); 
};

class StateBiting : public State
{
  void enter();
  State *loop();
  void exit(); 
};


// StateIdle implementation
// ************************

void StateIdle::enter()
{
  Serial.println(">>>>>>>>>>>>> StateIdle.enter()");
  digitalWrite(LED_BUILTIN, LOW);
}

State *StateIdle::loop()
{
  if (distance < 500) {
    return new StateWatching();
  }
  return NULL;
}

void StateIdle::exit()
{
  Serial.println("<<<<<<<<<<<<< StateIdle.exit()");
}


// StateWatching implementation
// *****************************

void StateWatching::enter()
{
  Serial.println(">>>>>>>>>>>>> StateWatching.enter()");

  // Turn on the LED
  digitalWrite(LED_BUILTIN, HIGH);

}

State *StateWatching::loop()
{
  if (speed > limit) {
    return new StateBarking();
  }

  if (distance > 500) {
    return new StateIdle();
  }

  if (distance < 200) {
    return new StateGreeting();
  }
  
  return NULL;
}

void StateWatching::exit()
{
  Serial.println("<<<<<<<<<<<<< StateWatching.exit()");
} 


// StateGreeting implementation
// *****************************

void StateGreeting::enter()
{
  Serial.println(">>>>>>>>>>>>> StateGreeting.enter()");
}

State *StateGreeting::loop()
{
  if (distance > 200) {
    return new StateWatching();
  }

  // Ramp the LED intensity up and down
  long period = 2 * 1000L;
  float intensity = (millis() % period) * 1.0 / period; // 0.0 ... 1.0
  
  // Custom non blocking PWM code for non PWM pin
  long pwmPeriod = 20 * 1000L; // microseconds
  long timePwm = micros() % pwmPeriod;
  bool high = timePwm < pwmPeriod * intensity;
  digitalWrite(LED_BUILTIN, high);

  return NULL;
}

void StateGreeting::exit()
{
  Serial.println("<<<<<<<<<<<<< StateGreeting.exit()");
}


// StateBarking implementation
// *****************************

void StateBarking::enter()
{
  Serial.println(">>>>>>>>>>>>> StateBarking.enter()");
  enterDistance = distance;
}

State *StateBarking::loop()
{
  // Go back to watching if the distance has increased by 100 cm since we entered this state
  if (distance > enterDistance + 100) {
    return new StateWatching();
  }

  if (distance < 200) {
    return new StateBiting();
  }

  // Blink the LED medium
  digitalWrite(LED_BUILTIN, millis() % 1000 < 50);


  return NULL;
}

void StateBarking::exit()
{
  Serial.println("<<<<<<<<<<<<< StateBarking.exit()");
}


// StateBiting implementation
// *****************************

void StateBiting::enter()
{
  Serial.println(">>>>>>>>>>>>> StateBiting.enter()");
}

State *StateBiting::loop()
{
  if (distance > 200) {
    return new StateBarking();
  }

  // Blink the LED fast
  digitalWrite(LED_BUILTIN, (millis() / 100) % 2);

  return NULL;
}

void StateBiting::exit()
{
  Serial.println("<<<<<<<<<<<<< StateBiting.exit()");
}
