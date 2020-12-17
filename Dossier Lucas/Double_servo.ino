//Lucas Bagrowski
//Projet Ruche
//Déplacement en boucle de 2 servomoteurs
///////////////////////////////////////
// include the Servo library
#include <Servo.h>

Servo ServoX;
Servo ServoY;

int iAngleX, iAngleY;   // variable to hold the angle for the servo motor



void setup() {
  // put your setup code here, to run once:
  ServoX.attach(10);
  ServoY.attach(11);
  Serial.begin(9600); // open a serial connection to your computer
}

void loop() {

  for(iAngleX= 0; iAngleX<180; iAngleX++)
  {
    // set the servo position
    ServoX.write(iAngleX);
    ServoY.write(iAngleX);
    delay(20);
  }

  for(iAngleX= 180; iAngleX>=0; iAngleX--)
  {
    // set the servo position
    ServoX.write(iAngleX);
    ServoY.write(iAngleX);
    delay(20);
  }
}
