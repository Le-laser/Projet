//Lucas Bagrowski
//Projet Ruche asservissement position
//Version 2 10/12/2020
//03/12/2020
//Réception de la position d'un frelon en X et Y à un instant T
//Gestion du déplacement des actionneurs
///////////////////////////////////////
//Include librairies and external files
#include <Servo.h>


//define
#define DEBUG 1 //Affichage de Débug 0: Pas d'affichage -- 1: Affichage
//Actionneurs utilisés
#define ACTIONNEURS 0 //0: Servomoteur -- 1: Stepmotor
//Entrée utilisée
#define CLAVIER 1
//Données de la caméra
#define PIXELSX 800 //Nombre de pixels en X de la caméra
#define ANGLECAMX 66 //Angle de vue de la caméra en X [°]
//Données servomoteur
#define PSERVO1 10
#define TIMESTEPSERVO 5 //Temps laissé entre chaque déplacement d'un pas d'un servomoteur [ms]
#define ANGLEMINSERVO 0 //Angle minimum d'un servomoteur [°]
#define ANGLEMAXSERVO 180 //Angle maximum d'un servomoteur [°]
#define ANGLECENTREX 90 //Angle de centrage axe X
//Données Stepmoteurs
#define DIRPIN1 6 //Broche direction moteur 1
#define STEPPIN1 7 //Broche pas moteur 1
#define DIRPIN2 4 //Broche direction moteur 2
#define STEPPIN2 3 //Broche pas moteur 2
#define STEPSPERREV 3200 //Nombre de pas par tour de moteur pas à pas
#define HALFTIMESTEPMOTOR 2000 //Temps d'un demi-pas de moteur pas à pas [µs]


//Objets ??
Servo ServoX;


//Constantes


//Variables
float fErreurPiX= 0, fErreurAngX = 0;
int iCenterX= 0;
int iAngleServoX= 90; //Angle actuel et de démarrage
float fRapportAngPiX= 0; //Rapport angle/pixel de la caméra en X

int iPosXCam= 0, iPosXCam_Av= 0; //Valeur de Position en X à analyser

float fRapportPasAngMot= 0; //Rapport angle/pas des moteurs pas à pas


//Structures


//Prototypes
void AcquisitionClavier();
void ReceptionPos();
void Move(int, int, int);


///////////////   Setup   ///////////////
void setup() {
  Serial.begin(9600); // open a serial connection to your computer

  ServoX.attach(PSERVO1);

  //Caméra
  //Calcul du centre des pixels de la caméra
  iCenterX = PIXELSX / 2;
  //Calcul rapport angle/pixels de la caméra
  fRapportAngPiX =  (float)ANGLECAMX / (float)PIXELSX;

  //Servo
  //Centrage du servomoteur
  if(ACTIONNEURS == 0) ServoX.write(ANGLECENTREX);

  //Stepmoteur
  //déclaration des broches
  pinMode(DIRPIN1, OUTPUT);
  pinMode(STEPPIN1, OUTPUT);
  pinMode(DIRPIN2, OUTPUT);
  pinMode(STEPPIN2, OUTPUT);
  //Calcul du rapport Angle Pas
  if(ACTIONNEURS == 1)
  {
    fRapportPasAngMot= 360.0/HALFTIMESTEPMOTOR;
  }
}


///////////////   Loop   ///////////////
void loop() {
  AcquisitionClavier();

  //Test de mise à jour de la valeur acquise
  if (iPosXCam != iPosXCam_Av)
  {
    Move(iPosXCam, 0, 1);
    iPosXCam_Av = iPosXCam;
  }
  
}


/////////////   Fonctions  ////////////

//
//Fonction non bloquante d'acquisition de touche
void AcquisitionClavier()
{
  int iByteAcq = 10; //Touche acquise par la liaison série

  if (Serial.available() > 0)
  {
    // read the incoming byte:
    iByteAcq = Serial.read();

    //On supprime les ENTER présents à chaque fin de transission
    if (iByteAcq != 10)
    {
      //Affichage de la Touche appuyée
      if (DEBUG) Serial.print("I received: ");
      if (DEBUG) Serial.println((char)iByteAcq);

      iPosXCam = iByteAcq;
    }
  }
}


//
//
void ReceptionPos()
{

}


//
//
void Move(int iPosX, int iPosY)
{
  //Si je choisis d'utiliser une autre acquisition que le clavier
  if (CLAVIER == 0)
  {
    //Calcul de l'erreur de position en pixels avec comme référence le centre
    fErreurPiX = iPosX - iCenterX;
    if (DEBUG) Serial.print("f Erreur Pixels= ");
    if (DEBUG) Serial.println(fErreurPiX);

    //Conversion de l'erreur en un Angle
    fErreurAngX = fErreurPiX * fRapportAngPiX;
    if (DEBUG) Serial.print("f Erreur Angle= ");
    if (DEBUG) Serial.println(fErreurAngX);

    //Suppression des décimales
    fErreurAngX = (int)fErreurAngX;
    if (DEBUG) Serial.print("f Erreur Angle int= ");
    if (DEBUG) Serial.println(fErreurAngX);
  }

  
  //Si on utilise les servomoteurs
  if(ACTIONNEURS == 0)
  {
    //Si je choisis d'utiliser l'acquisition clavier
    if (CLAVIER == 1)
    {
      if ( '1' <= (char)iPosX && (char)iPosX <= '9')
      {
        //Conversion de la touche appuyée en position sur la caméra
        iPosX = 57 + ((float)(iPosX - 48) * ((123 - 57) / 9));
        if (DEBUG) Serial.print("iPosX convertie clavier=");
        if (DEBUG) Serial.println(iPosX);
  
        fErreurAngX = iPosX - iAngleServoX;
      }
    }
  
    //Tant que le laser n'est pas aligné on boucle
    while (fErreurAngX != 0)
    {
      //S'il faut aller à droite
      if (fErreurAngX > 0)
      {
        //Vérification de non dépassement
        if (iAngleServoX >= ANGLEMAXSERVO) Serial.println("Angle X max atteint");
        else
        {
          iAngleServoX ++;
          fErreurAngX --;
        }
      }
      //S'il faut aller à gauche
      else
      {
        if (iAngleServoX <= ANGLEMINSERVO) Serial.println("Angle X min atteint");
        else
        {
          iAngleServoX --;
          fErreurAngX ++;
        }
      }
      if (DEBUG) Serial.println("Mouvement servo");
      ServoX.write(iAngleServoX);
      delay(TIMESTEPSERVO);
    }
  }

  //Si on utilise les moteurs pas à pas
  else
  {
    //Tant que le laser n'est pas aligné on boucle
    while (fErreurAngX != 0)
    {
      //S'il faut aller à droite
      if (fErreurAngX > 0)
      {
        digitalWrite(DIRPIN1, HIGH);
        fErreurAngX -= fRapportPasAngMot;
      }
      //S'il faut aller à gauche
      else
      {
        digitalWrite(DIRPIN1, LOW);
        fErreurAngX += fRapportPasAngMot;
      }
      digitalWrite(STEPPIN1, HIGH);
      delayMicroseconds(HALFTIMESTEPMOTOR);
      digitalWrite(STEPPIN1, LOW);
      delayMicroseconds(HALFTIMESTEPMOTOR);
    }
  }
}
