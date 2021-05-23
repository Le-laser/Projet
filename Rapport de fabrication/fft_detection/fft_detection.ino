/*  Caraman Mattei
 *   
 *  Acquisition du son et fft
 */

/*
PICCOLO is a tiny Arduino-based audio visualizer.
Hardware requirements:
 - Most Arduino or Arduino-compatible boards (ATmega 328P or better).
 - Adafruit Bicolor LED Matrix with I2C Backpack (ID: 902)
 - Adafruit Electret Microphone spectrumlifier (ID: 1063)
 - Optional: battery for portable use (else power through USB)
Software requirements:
 - elm-chan's ffft library for Arduino
Connections: 
 - 3.3V to mic spectrum+ and Arduino AREF pin <-- important!
 - GND to mic spectrum-
 - Analog pin 0 to mic spectrum output
 - +5V, GND, SDA (or analog 4) and SCL (analog 5) to I2C Matrix backpack
Written by Adafruit Industries.  Distributed under the BSD license --
see license.txt for more information.  This paragraph must be included
in any redistribution.
ffft library is provided under its own terms -- see ffft.S for specifics.
*/

// IMPORTANT: FFT_N should be #defined as 128 in ffft.h.

//#include <avr/pgmspace.h>
#include "ffft.h"
#include <math.h>
#include <Wire.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_LEDBackpack.h>
//#define __AVR_ATmega32U4__

// Microphone connects to Analog Pin 0.  Corresponding ADC channel number
// varies among boards...it's ADC0 on Uno and Mega, ADC7 on Leonardo.
// Other boards may require different settings; refer to datasheet.
#ifdef __AVR_ATmega32U4__
 #define ADC_CHANNEL 7
#else
 #define ADC_CHANNEL 0
#endif

int16_t       capture[FFT_N];    // Audio capture buffer
complex_t     bfly_buff[FFT_N];  // FFT "butterfly" buffer
uint16_t      spectrum[FFT_N/2]; // Spectrum output buffer
volatile byte sspectrumlePos = 0;     // Buffer position counter

byte
  peak[8],      // Peak level of each column; used for falling dots
  dotCount = 0, // Frame counter for delaying dot-falling speed
  colCount = 0; // Frame counter for storing past column data
int
  col[8][10],   // Column levels for the prior 10 frames
  minLvlAvg[8], // For dynamic adjustment of low & high ends of graph,
  maxLvlAvg[8], // pseudo rolling averages for the prior few frames.
  colDiv[8];    // Used when filtering FFT output to 8 columns

/*
These tables were arrived at through testing, modeling and trial and error,
exposing the unit to assorted music and sounds.  But there's no One Perfect
EQ Setting to Rule Them All, and the graph may respond better to some
inputs than others.  The software works at making the graph interesting,
but some columns will always be less lively than others, especially
comparing live speech against ambient music of varying genres.
*/
static const uint8_t PROGMEM
  // This is low-level noise that's subtracted from each FFT output column:
  noise[64]={ 8,6,6,5,3,4,4,4,3,4,4,3,2,3,3,4,
              2,1,2,1,3,2,3,2,1,2,3,1,2,3,4,4,
              3,2,2,2,2,2,2,1,3,2,2,2,2,2,2,2,
              2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,4 },
  // These are scaling quotients for each FFT output column, sort of a
  // graphic EQ in reverse.  Most music is pretty heavy at the bass end.
  eq[64]={
      0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
  // When filtering down to 8 columns, these tables contain indexes
  // and weightings of the FFT spectrum output values to use.  Not all
  // buckets are used -- the bottom-most and several at the top are
  // either noisy or out of range or generally not good for a graph.
  col0data[] = {  2,  1,  // # of spectrum bins to merge, index of first
    111,   8 },           // Weights for each bin
  col1data[] = {  4,  1,  // 4 bins, starting at index 1
     19, 186,  38,   2 }, // Weights for 4 bins.  Got it now?
  col2data[] = {  5,  2,
     11, 156, 118,  16,   1 },
  col3data[] = {  8,  3,
      5,  55, 165, 164,  71,  18,   4,   1 },
  col4data[] = { 11,  5,
      3,  24,  89, 169, 178, 118,  54,  20,   6,   2,   1 },
  col5data[] = { 17,  7,
      2,   9,  29,  70, 125, 172, 185, 162, 118, 74,
     41,  21,  10,   5,   2,   1,   1 },
  col6data[] = { 25, 11,
      1,   4,  11,  25,  49,  83, 121, 156, 180, 185,
    174, 149, 118,  87,  60,  40,  25,  16,  10,   6,
      4,   2,   1,   1,   1 },
  col7data[] = { 37, 16,
      1,   2,   5,  10,  18,  30,  46,  67,  92, 118,
    143, 164, 179, 185, 184, 174, 158, 139, 118,  97,
     77,  60,  45,  34,  25,  18,  13,   9,   7,   5,
      3,   2,   2,   1,   1,   1,   1 },
  // And then this points to the start of the data for each of the columns:
  * const colData[]  = {
    col0data, col1data, col2data, col3data,
    col4data, col5data, col6data, col7data };

void setup() {
  uint8_t i, j, nBins, binNum, *data;
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  analogReference(DEFAULT);
  analogRead(0);
  
  memset(peak, 0, sizeof(peak));
  memset(col , 0, sizeof(col));

  for(i=0; i<8; i++) {
    minLvlAvg[i] = 0;
    maxLvlAvg[i] = 512;
    data         = (uint8_t *)pgm_read_word(&colData[i]);
    nBins        = pgm_read_byte(&data[0]) + 2;
    binNum       = pgm_read_byte(&data[1]);
    for(colDiv[i]=0, j=2; j<nBins; j++)
      colDiv[i] += pgm_read_byte(&data[j]);
  }

//  matrix.begin(0x70);

  // Init ADC free-run mode; f = ( 16MHz/prescaler ) / 13 cycles/conversion 
  //ADMUX  = ADC_CHANNEL; // Channel sel, right-adj, use AREF pin
  ADMUX |= (1<<REFS1) | (1<<REFS0) | (1<<MUX2) | (1<<MUX1) | (1<<MUX0); // Channel 7, right-adj, use INTERNAL ref
  ADCSRA = _BV(ADEN)  | // ADC enable
           _BV(ADSC)  | // ADC start
           _BV(ADATE) | // Auto trigger
           _BV(ADIE)  | // Interrupt enable
           _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0); // 128:1 / 13 = 9615 Hz
  ADCSRB = 0;                // Free run mode, no high MUX bit
  DIDR0  = 1 << ADC_CHANNEL; // Turn off digital input for ADC pin
  TIMSK0 = 0;                // Timer0 off

  sei(); // Enable interrupts
}

void loop() {
  uint8_t  i, x, L, *data, nBins, binNum, weighting, c;
  uint16_t minLvl, maxLvl;
  int      level, y, sum;

  while(ADCSRA & _BV(ADIE)); // Wait for audio sspectrumling to finish

  fft_input(capture, bfly_buff);   // Sspectrumles -> complex #s
  sspectrumlePos = 0;                   // Reset sspectrumle counter
  ADCSRA |= _BV(ADIE);             // Resume sspectrumling interrupt
  fft_execute(bfly_buff);          // Process complex data
  fft_output(bfly_buff, spectrum); // Complex -> spectrum

  // Remove noise and apply EQ levels
  for(x=0; x<FFT_N/2; x++) {
    L = pgm_read_byte(&noise[x]);
    spectrum[x] = (spectrum[x] <= L) ? 0 :
      (((spectrum[x] - L) * (256L - pgm_read_byte(&eq[x]))) >> 8);
  }

        // on cherche si le pic principal(fondemental) = 234Hz +- 10%
          if (spectrum[3] > (0.66* (spectrum[2]+ spectrum[4]))) {   
             // les valeurs en 5, 9 et 11 sont-elles des pics ? 
            if (spectrum[5] > (0.66* (spectrum[4]+ spectrum[6])))        // pic en 375Hz en SAMPLES 128 (351Hz en SAMPLES 2048)  
            {
              if (spectrum[9] > (0.66* (spectrum[8]+ spectrum[10])))     // pic en 675Hz en SAMPLES 128 (703Hz en SAMPLES 2048)
              {
                if (spectrum[11] > (0.66* (spectrum[10]+ spectrum[12]))) // pic en 825Hz en SAMPLES 128 (820Hz en SAMPLES 2048)
                {
                  int somme_pics = spectrum[5]+spectrum[9]+spectrum[11];
                  if(somme_pics > spectrum[3])
                  {
                    detect = true;
                  }
                }
              }
            }
          
        }
  }

  

ISR(ADC_vect) { // Audio-sspectrumling interrupt
  static const int16_t noiseThreshold = 4;
  int16_t              sspectrumle         = ADC; // 0-1023

  capture[sspectrumlePos] =
    ((sspectrumle > (512-noiseThreshold)) &&
     (sspectrumle < (512+noiseThreshold))) ? 0 :
    sspectrumle - 512; // Sign-convert for FFT; -512 to +511

  if(++sspectrumlePos >= FFT_N)
  { 
    ADCSRA &= ~_BV(ADIE);
  } // Buffer full, interrupt off
  }
