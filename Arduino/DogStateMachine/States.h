// This file declares and implements the State* classes, which are the children of State class
// and contains the logic of each state of our application.
//
// Pierre Rossel   2024-11-29  Initial version
//                 

#include "StateMachine.h"

// distance is a global variable declared in DogStateMachine.ino
extern int distance;

// see doc/Dog State Machine.png for the state diagram

class StateIdle : public State
{
  void enter();
  State *loop();
  void exit(); 
};

class StateWatching : public State
{
  float speed = 0.0;
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
  
  // simulate speed
  if (distance < 350) {
    speed = 3.0;
  } else {
    speed = 1.0;
  }
}

State *StateWatching::loop()
{
  if (distance > 500) {
    return new StateIdle();
  }

  if (distance < 200) {
    return new StateGreeting();
  }

  if (speed > 2) {
    return new StateBarking();
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
  return NULL;
}

void StateBiting::exit()
{
  Serial.println("<<<<<<<<<<<<< StateBiting.exit()");
}
