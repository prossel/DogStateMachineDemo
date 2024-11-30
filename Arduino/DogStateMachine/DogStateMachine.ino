// DogStateMachine demo
//
// By using the StateMachine class and your own state classes herited from the State base class,
// we can easily code an application that has different tasks to do at different times and separate
// them in the state classes.
//
// Pierre Rossel   2024-11-29  Initial version

#include "States.h"

int distance = 1000;

// mode of operation
enum Mode {
  ANALOG,    // read distance from analog input
  SIM_SERIAL // read distance from serial input
};

Mode mode = SIM_SERIAL;

StateMachine sm;

void setup() {
  Serial.begin(9600);

  Serial.println("\n\nHello world of state machines. This is the DogStateMachine example.");
  Serial.println("\n\nSend a distance (0 .. 1023) in cm to simulate a distance sensor ");
  Serial.println("or an empty line to go back to reading the distance on an analog input.");

  sm.changeState(new StateIdle());
}

void loop() {

  switch (mode) {

    case ANALOG:

      distance = analogRead(A0);
      if (Serial.available() > 0) {
        mode = SIM_SERIAL;
        Serial.println("Switching to simulation mode.");
      } else {
        break;
      }

    case SIM_SERIAL:
    
      if (Serial.available() > 0) {
        
        // Try to read an int
        distance = Serial.parseInt();

        // flush the serial buffer
        while (Serial.available() > 0) {
          Serial.read();
        }

        // could not read an int (or 0)
        if (distance == 0) {
          mode = ANALOG;
          Serial.println("Switching to analog mode.");
        }
      }
      break;
  }

  // Serial.print("Distance:");
  // Serial.println(distance);

  sm.loop();

  delay(100);
}
