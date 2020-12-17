//Lucas Bagrowski
//17/12/2020
//Test Communication I2C entre 2 Arduinos
//Master
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
void EnvoiTrame();


///////////////   Setup   ///////////////
void setup() {
  Wire.begin(); // join i2c bus
  Serial.begin(9600);

  iTabTrame[POSCARACSPE]= CARACSPE;
}


///////////////   Loop   ///////////////
void loop() {

  iTabTrame[POSITIONX]= 157;
  iTabTrame[POSITIONY]= 423;

  Serial.println(" ");
  Serial.print("Position X= ");
  Serial.print(iTabTrame[POSITIONX]);
  Serial.print("     Postion Y= ");
  Serial.print(iTabTrame[POSITIONY]);
  
  EnvoiTrame();
  
  delay(333);
}


/////////////   Fonctions  ////////////
void EnvoiTrame()
{
  Serial.print("Envoi");
  //Envoi de la Trame en I2C
  Wire.beginTransmission(ADDRESSEI2CNANO);
  for(int i= 0; i<LONGTRAME; i++)
  { 
    Wire.write(iTabTrame[i]);
  }
  Wire.endTransmission();
}
