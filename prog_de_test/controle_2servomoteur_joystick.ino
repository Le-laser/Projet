#include <Servo.h>

Servo ServoX;
Servo ServoY;

#define AXEX A6
#define AXEY A7
#define SWITCH A3


int iAxeX= 560;
int iAxeY= 560;
int iAngleX= 0;
int iAngleY= 0;

void setup() {
  pinMode(AXEX, INPUT);
  pinMode(AXEY, INPUT);
  pinMode(SWITCH, INPUT);
  
  ServoX.attach(10);
  ServoY.attach(9);

  Serial.begin(9600);
}

void loop() {
  iAxeX= analogRead(AXEX);
  iAngleX= iAxeX/10;
  Serial.println(iAngleX);

  iAxeY= analogRead(AXEY);
  iAngleY= (iAxeY+800)/10;
  Serial.println(iAngleY);
  
  ServoX.write(iAngleX);
  ServoY.write(iAngleY);
  //delay(200);
}
