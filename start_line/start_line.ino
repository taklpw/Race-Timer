/**
 * @author   Kelly Lynch
 * @date     2017-02-23
 * @filename start_line.ino
 * 
 * @brief    Software for a timer that measures how fast someone runs an arbitray distance.
 *           This software is paired with the finish_line.ino file and associated hardware.
 *          
 *           This software uses the following hardware:
 *           - Arduino Nano V3
 *           - NRF24L01+PA+LNA for wireless communication
 *           - Sharp GP2Y0A02YK0F IR distance sensor for detecting on the finish line
 *           - Push Button for starting the race
 *           - Red LED for visual cues
 *           - 8Ohm 1W speaker for audible starting sound
 *           
 * @todo     Add Break condition
 *           Configure handshake for wireless
 *           Add force sensor to measure reaction time
 *          
*/

/* --Includes-- */
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

/* CE on pin 7, CSN on pin 8*/
RF24 radio(7, 8);

/* --Constants-- */
const byte rxAddr[6] = "00001";
const int buttonPin = 9;
const int speakerPin = A0;
const int ledPin = 10;

/* --Global Variables-- */
boolean started = false;
boolean firstRun = true;
unsigned long startTime = 0;
unsigned long endTime = 0;
float secondsVal = 0;

/**
 * @name setup
 * @brief Initialises wireless settings, serial settings, and digital inputs/outputs. Runs once.
 * @param void
 * @return void
 */
void setup(){
  Serial.begin(9600);
  
  radio.begin();
  radio.openReadingPipe(0, rxAddr);
  radio.startListening(); 

  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

/**
 * @name Main loop
 * @brief Runs continually
 * @param void
 * @return void
 */
void loop(){
  /* If the start Button is pressed record information*/
  if(digitalRead(buttonPin) && started == false){
    startRace();  
    Serial.println("Started");
  }

  /* Radio reading*/
  char text[32] = {0};
  if (radio.available()){
    radio.read(&text, sizeof(text));
  }

  /* If race has finished record information*/
  if(text[0] == 'd' && started == true){
    finishRace(); 
    Serial.println("Finished!");
    
    /* Get the final time in seconds*/
    secondsVal = microsToSeconds(endTime - startTime);
    Serial.print("Result: ");
    Serial.print(secondsVal);
    Serial.print(" Seconds");
    Serial.print("\n\n");
  }
  
  delay(15);
}

/**
 * @name Start Race
 * @brief Starts the race conditions by recording the time and giving visual and audible indications.
 * @param void
 * @return void
 */
void startRace(){
  startTime = micros();
  digitalWrite(ledPin, HIGH);
  tone(speakerPin, 1000, 500);
  started = true;
}


/**
 * @name Finish Race
 * @brief Finishes the race by recording the time and giving visual and audible indications.
 * @param void
 * @return void
 */
void finishRace(){
  endTime = micros();
  digitalWrite(ledPin, LOW);
  tone(speakerPin, 1500, 200);
  started = false;
}

/**
 * @name Finish Race
 * @brief Convert a microsecond value to a seconds value by dividing by 1 million.
 * @param unisgned long microsecond value
 * @return float second value
 */
float microsToSeconds(unsigned long microsVal){
  return microsVal/1000000.0;
}


