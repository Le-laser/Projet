/* Mattei Caraman

   Reception de la fft par port COM
   Detection des frelons
   Affichage du spectre de raies
   Creation d'un fichier csv pour stoquer les raies
*/

/* 
  Saving Values from Arduino to a .csv File Using Processing - Pseduocode

  This sketch provides a basic framework to read data from Arduino over the serial port and save it to .csv file on your computer.
  The .csv file will be saved in the same folder as your Processing sketch.
  This sketch takes advantage of Processing 2.0's built-in Table class.
  This sketch assumes that values read by Arduino are separated by commas, and each Arduino reading is separated by a newline character.
  Each reading will have it's own row and timestamp in the resulting csv file. This sketch will write a new file a set number of times. Each file will contain all records from the beginning of the sketch's run.  
  This sketch pseduo-code only. Comments will direct you to places where you should customize the code.
  This is a beginning level sketch.

  The hardware:
  * Sensors connected to Arduino input pins
  * Arduino connected to computer via USB cord
        
  The software:
  *Arduino programmer
  *Processing (download the Processing software here: https://www.processing.org/download/
  *Download the Software Serial library from here: http://arduino.cc/en/Reference/softwareSerial

  Created 12 November 2014
  By Elaine Laguerta
  http://url/of/online/tutorial.cc

*/

import processing.serial.*;
Serial myPort; //creates a software serial port on which you will listen to Arduino
Table dataTable = new Table(); //table where we will read in and store values. You can name it something more creative!

int numReadings = 500; //keeps track of how many readings you'd like to take before writing the file. 
int readingCounter = 0; //counts each reading to compare to numReadings. 
int spectrumIndex=1;
int init,offset = 0;
int[] amp = new int [64]/*{5,4,7,12,15,50,25}*/ ; //holds next spectrum amplitude values
int[] freq = new int [64] ;  //holds next spectrum frequency values
boolean detect = false;

String val;  //stores fft samples
String fft; // file for storing fft values
void setup()
{
  String portName = Serial.list()[0]; //arduino -> com5
  //CAUTION: your Arduino port number is probably different! Mine happened to be 1. Use a "handshake" sketch to figure out and test which port number your Arduino is talking on. A "handshake" establishes that Arduino and Processing are listening/talking on the same port.
  //Here's a link to a basic handshake tutorial: https://processing.org/tutorials/overview/
  
  myPort = new Serial(this, portName, 115200); //set up your port to listen to the serial port
   
  dataTable.addColumn("id"); //This column stores a unique identifier for each record. We will just count up from 0 - so your first reading will be ID 0, your second will be ID 1, etc. 
  
  /*//the following adds columns for time. You can also add milliseconds. See the Time/Date functions for Processing: https://www.processing.org/reference/ 
  dataTable.addColumn("year");
  dataTable.addColumn("month");
  dataTable.addColumn("day");
  dataTable.addColumn("hour");
  dataTable.addColumn("minute");
  dataTable.addColumn("second");
  */
  
  //the following are dummy columns for each data value. Add as many columns as you have data values. Customize the names as needed. Make sure they are in the same order as the order that Arduino is sending them!
  dataTable.addColumn("Frequence");
  dataTable.addColumn("Amplitude");
  
  frameRate(20);//for drawing graph 
  size(1280, 255);//window size

}

void serialEvent(Serial myPort){
  try{
  val = myPort.readStringUntil('\n'); //The newline separator separates each Arduino loop. We will parse the data by each newline separator. 
  if (val!= null) { //We have a reading! Record it.
    val = trim(val); //gets rid of any whitespace or Unicode nonbreakable space
    //println(val); //Optional, useful for debugging. If you see this, you know data is being sent. Delete if  you like. 
    int[] sensorVals = int(split(val, ',')); //parses the packet from Arduino and places the valeus into the sensorVals array. I am assuming floats. Change the data type to match the datatype coming from Arduino. 
     //printArray(sensorVals);
     TableRow row;
     
     if(init == 0 ){
        init = 1;
      }
     
     // If the table has more than 640 rows
      if (dataTable.getRowCount() > 640) 
      {
        //Delete the first row (index 0).
        dataTable.removeRow(0);
      }
     
    TableRow newRow = dataTable.addRow(); //add a row for this new reading
    newRow.setInt("id", dataTable.lastRowIndex());//record a unique identifier (the row's index)
    
    /*//record time stamp
    newRow.setInt("year", year());
    newRow.setInt("month", month());
    newRow.setInt("day", day());
    newRow.setInt("hour", hour());
    newRow.setInt("minute", minute());
    newRow.setInt("second", second());
    */
    
    //record sensor information. Customize the names so they match your sensor column names
    newRow.setInt("Frequence", sensorVals[0]);
    newRow.setInt("Amplitude", sensorVals[1]);
    
    if(dataTable.getRowCount()>128){ //wait to receive at least 2 spectrum samples
      for (int i=0; i<64; i++) {
        row = dataTable.getRow(spectrumIndex*64+i);
        if (row.getInt("Frequence") == 0 ){ //get offset from beggining of spectrum
            offset = i;
        }
      }
      
      for (int i=0; i<64; i++) {
        row = dataTable.getRow(spectrumIndex*64+i-64+offset);
        amp[i] = row.getInt("Amplitude");
        freq[i] = row.getInt("Frequence");
      }
      
      // on cherche si le pic = 234 +- 10%
      //if (pic1 >210  && pic1 <=260) {   
      if (amp[3] > (0.66* (amp[2]+ amp[4]))) {   
         // les valeurs en 5, 9 et 11 sont-elles des pics ? 
        if (amp[5] > (0.66* (amp[4]+ amp[6])))        // pic en 375 en SAMPLES 128 (351 en SAMPLES 2048)  
        {
          if (amp[9] > (0.66* (amp[8]+ amp[10])))     // pic en 675 en SAMPLES 128 (703 en SAMPLES 2048)
          {
            if (amp[11] > (0.66* (amp[10]+ amp[12]))) // pic en 825 en SAMPLES 128 (820 en SAMPLES 2048)
            {
              int somme_pics = amp[5]+amp[9]+amp[11];
              if(somme_pics > amp[3])
              {
                detect = true;
              }
            }
          }
          
        }
      
    }
    
    readingCounter++; //optional, use if you'd like to write your file every numReadings reading cycles
    if(spectrumIndex<9)spectrumIndex++; //index for FFT spectrum in arrray
    
    //saves the table as a csv in the same folder as the sketch every numReadings. 
    if (readingCounter % numReadings ==0)//The % is a modulus, a math operator that signifies remainder after division. The if statement checks if readingCounter is a multiple of numReadings (the remainder of readingCounter/numReadings is 0)
    {
      fft =/* str(year()) + str(month()) + str(day()) + */str(dataTable.lastRowIndex()); //this filename is of the form year+month+day+readingCounter
      saveTable(dataTable, "donnees.csv"); //Woo! save it to your computer. It is ready for all your spreadsheet dreams. 
    }
   }
  }
 }//try catch
 catch(RuntimeException e) {
 e.printStackTrace();
 }
}

void draw()
{ 
   //visualize your sensor data in real time here! In the future we hope to add some cool and useful graphic displays that can be tuned to different ranges of values. 
   background(255);
  stroke(0);
  strokeWeight(2);
  //if(readingCounter % 64 == 0 && readingCounter != 0){
    //printArray(amp);
    if(dataTable.getRowCount() > 128){ //get at least 2 FFT spectrum samples
      for (int i=0; i<64; i++) {
         //println(amp[i]);
        // Use array of ints to set the color and height of each rectangle.
        rect(i*20, 220, 20, -amp[i]);
        fill(0);
        textSize(6); 
        textAlign(CENTER);
        text(freq[i], i*20+10, 230);
        if (detect == true )
        {
          textSize(10); 
          textAlign(CENTER);
          text("frelon detecte:",720 , 120);
          detect = false;
        }
      }
      //else if( dataTable.getRowCount() == 640) spectrumIndex = 9;
    }
  //}  
  //noLoop();
}
