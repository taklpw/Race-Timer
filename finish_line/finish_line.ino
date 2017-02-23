/**
 * @author   Kelly Lynch
 * @date     2017-02-23
 * @filename finish_line.ino
 * 
 * @brief    Software for a timer that measures how fast someone runs an arbitray distance.
 *           This software is paired with the start_line.ino file and associated hardware.
 *          
 *           This software uses the following hardware:
 *           - Arduino Nano V3
 *           - NRF24L01+PA+LNA for wireless communication
 *           - Sharp GP2Y0A02YK0F IR distance sensor for detecting on the finish line
 *           
 * @todo     Configure handshake for wireless
 *          
*/

/* --Includes-- */
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

/* CE on pin 7, CSN on pin 8 */
RF24 radio(7, 8);

/* --Constants-- */
const byte rxAddr[6] = "00001";
const int thresh = 200;
const int samples = 10;
const char detected[] = "d";
const char notDetected[] = "n";

/* --Varibles-- */
int loopVal = 0;
int total = 0;
int averageVal = 0;

/**
 * @name setup
 * @brief Initialises wireless settings and serial settings. Runs once.
 * @param void
 * @return void
 */
void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setRetries(15,15); 
  radio.openWritingPipe(rxAddr);
  radio.stopListening();

}

/**
 * @name Main Loop
 * @brief Runs continually
 * @param void
 * @return void
 */
void loop() {  
  /* Collect Average values of IR sensor */
  if(samples > loopVal){
    sumTotal(analogRead(A0));
  }else{
    /* Calculate the average value */
    averageVal = calcAverage();  
    
    /* Transmit data based on weather or not it has been tripped */
    if(averageVal > thresh){
      radio.write(&detected, sizeof(detected));  
      Serial.println("Detected");
    }else{
      radio.write(&notDetected, sizeof(notDetected));        
      Serial.println("Not Detected");  
    }
    
    /* Reset Values for calculating averages */
    total = 0;
    loopVal = 0;
  }
}

/**
 * @name Sum Total
 * @brief Adds IR input sample to a total and increments loop value.
 * @param void
 * @return void
 */
void sumTotal(int irInput){
  total += irInput;
  loopVal += 1;
}

/**
 * @name Calculate Average
 * @brief Calculates the average value
 * @param void
 * @return integer average value
 */
int calcAverage(){
  return total / samples;  
}
