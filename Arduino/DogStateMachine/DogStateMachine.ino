// DogStateMachine demo
//
// By using the StateMachine class and your own state classes herited from the State base class,
// we can easily code an application that has different tasks to do at different times and separate
// them in the state classes.
//
// Pierre Rossel   2024-11-29  Initial version

#include "States.h"

int distance = 1000;
float speed = 0;

// speed calculation
int previousDistance = 0;
unsigned long previousMs = 0;

// mode of operation
enum Mode {
  ANALOG,     // read distance from analog input
  SIM_SERIAL  // read distance from serial input
};

Mode mode = ANALOG;

StateMachine sm;

void setup() {
  Serial.begin(9600);

  Serial.println("\n\nHello world of state machines. This is the DogStateMachine example.");
  Serial.println("\n\nSend a distance (0 .. 1023) in cm to simulate a distance sensor ");
  Serial.println("or an empty line to go back to reading the distance on an analog input.");

  pinMode(LED_BUILTIN, OUTPUT);

  sm.changeState(new StateIdle());
}

void loop() {
  static unsigned long lastAnalogRead = 0;
  unsigned long ms = millis();

  switch (mode) {

    case ANALOG:

      // Non blocking code every 100 ms (avoir using delay())
      if (ms - lastAnalogRead > 100) {
        lastAnalogRead = ms;

        distance = analogRead(A0);

        updateSpeed();
      }

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

        updateSpeed();
      }
      break;
  }

  // Serial.print("Distance:");
  // Serial.println(distance);

  sm.loop();
}

void updateSpeed() {
  unsigned long ms = millis();

  if (previousMs == 0) {
    previousMs = ms;
    previousDistance = distance;
    return;
  }

  unsigned long deltaMs = ms - previousMs;
  int deltaDistance = distance - previousDistance;

  speed = -deltaDistance / (deltaMs / 1000.0);

  previousMs = ms;
  previousDistance = distance;

  Serial.print("Distance:");
  Serial.print(distance);
  Serial.print("\t");

  Serial.print("Speed:");
  Serial.println(speed);
}