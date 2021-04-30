//Lucas Bagrowski
//29/01/2021
//Test Communication UART entre 2 Arduinos
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



//Variables
int iTabTrame[LONGTRAME];


//Structures


//Prototypes
void EnvoiTrame();


///////////////   Setup   ///////////////
void setup() {
  Serial.begin(9600);

  iTabTrame[POSCARACSPE]= CARACSPE;
}


///////////////   Loop   ///////////////
void loop() {

  iTabTrame[POSITIONX]= 157;
  iTabTrame[POSITIONY]= 423;

  //Serial.println(" ");
  //Serial.print("Position X= ");
  //Serial.print(iTabTrame[POSITIONX]);
  //Serial.print("     Postion Y= ");
  //Serial.print(iTabTrame[POSITIONY]);
  
  EnvoiTrame();
  
  delay(5000);
}


/////////////   Fonctions  ////////////
void EnvoiTrame()
{
  Serial.print("Envoi");
  //Envoi de la Trame en I2C
  //Wire.beginTransmission(ADDRESSEI2CNANO);
  for(int i= 0; i<LONGTRAME; i++)
  { 
    //Wire.write(iTabTrame[i]);
    Serial.write(iTabTrame[i]);
  }
  //Wire.endTransmission();
}
