#define AXEX A5
#define AXEY A4
#define SWITCH A3

#define SEUILPX 
#define SEUILMX
#define SEUILPY
#define SEUILMY


int iAxeX= 0;
int iAxeY= 0;



void setup() {
  pinMode(AXEX, INPUT);
  pinMode(AXEY, INPUT);
  pinMode(SWITCH, INPUT);

  Serial.begin(9600);
}

void loop() {
  iAxeX= analogRead(AXEX);
  Serial.println(iAxeX);

  iAxeY= analogRead(AXEY);
  Serial.println(iAxeY);
  Serial.println();
  delay(200); 
}
