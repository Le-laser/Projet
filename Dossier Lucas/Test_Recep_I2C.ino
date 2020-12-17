//Lucas Bagrowski
//17/12/2020
//Test Communication I2C entre 2 Arduinos 
//Slave
//
///////////////////////////////////////
//Include librairies and external files
#include <Wire.h>


//Constantes 
#define LONGTRAME 3 //Nombre de cases dans la trame
#define POSCARACSPE 0 //Position du caractère spécial dans la trame
#define POSITIONX 1 //Position de la valeur X dans la trame
#define POSITIONY 2 //Position de la valeur X dans la trame
#define CARACSPE 35 //Caractère spécial signifiant le début de la trame 35 en ASCII = #
#define ADDRESSEI2CNANO 4 //Addresse I2C de notre Arduino nano


//Variables
int iTabTrame[LONGTRAME];


//Structures


//Prototypes
//void receiveEvent(int);
void Reception(int);


///////////////   Setup   ///////////////
void setup() {
  Wire.begin(ADDRESSEI2CNANO);
  Wire.onReceive(Reception); //.attach
  
  Serial.begin(9600); //Liaison série PC
}


///////////////   Loop   ///////////////
void loop() {
  delay(50);
}


/////////////   Fonctions  ////////////
void Reception(int howMany)
{
   while(1 < Wire.available())
  {
      
    if(Wire.read() == CARACSPE)
    {
      Serial.print('#');
      iTabTrame[POSITIONX]= Wire.read();
      Serial.print(iTabTrame[POSITIONX]);
      iTabTrame[POSITIONY]= Wire.read();
      Serial.print(iTabTrame[POSITIONY]); 
    }
    Serial.println();
  }
}
