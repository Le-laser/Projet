/*Example sketch to control a stepper motor with A4988 stepper motor driver and Arduino without a library. More info: https://www.makerguides.com */

// Define stepper motor connections and steps per revolution:
#define DIRPIN1 6
#define STEPPIN1 7

#define DIRPIN2 4
#define STEPPIN2 3

#define STEPSPERREV 3200


void setup() {
  // Declare pins as output:
  pinMode(DIRPIN1, OUTPUT);
  pinMode(STEPPIN1, OUTPUT);
  
  pinMode(DIRPIN2, OUTPUT);
  pinMode(STEPPIN2, OUTPUT);
}

void loop() {


  // Set the spinning direction clockwise:
  digitalWrite(DIRPIN1, HIGH);

  // Spin the stepper motor 1 revolution slowly:
  for (int i = 0; i < STEPSPERREV; i++) {
    // These four lines result in 1 step:
    digitalWrite(STEPPIN1, HIGH);
    delayMicroseconds(2000);
    digitalWrite(STEPPIN1, LOW);
    delayMicroseconds(2000);
  }

  delay(1000);


// Set the spinning direction clockwise:
  digitalWrite(DIRPIN1, LOW);

  // Spin the stepper motor 1 revolution slowly:
  for (int i = 0; i < STEPSPERREV; i++) {
    // These four lines result in 1 step:
    digitalWrite(STEPPIN1, HIGH);
    delayMicroseconds(2000);
    digitalWrite(STEPPIN1, LOW);
    delayMicroseconds(2000);
  }

  delay(1000);

  // Set the spinning direction clockwise:
  digitalWrite(DIRPIN2, HIGH);

  // Spin the stepper motor 1 revolution slowly:
  for (int i = 0; i < STEPSPERREV; i++) {
    // These four lines result in 1 step:
    digitalWrite(STEPPIN2, HIGH);
    delayMicroseconds(2000);
    digitalWrite(STEPPIN2, LOW);
    delayMicroseconds(2000);
  }

  delay(1000);


  // Set the spinning direction clockwise:
  digitalWrite(DIRPIN2, LOW);

  // Spin the stepper motor 1 revolution slowly:
  for (int i = 0; i < STEPSPERREV; i++) {
    // These four lines result in 1 step:
    digitalWrite(STEPPIN2, HIGH);
    delayMicroseconds(2000);
    digitalWrite(STEPPIN2, LOW);
    delayMicroseconds(2000);
  }

  delay(1000);
  
}
