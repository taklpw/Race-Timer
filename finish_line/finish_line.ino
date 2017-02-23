#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8);

const byte rxAddr[6] = "00001";

int thresh = 200;
int samples = 10;
int loopVal = 0;
int total = 0;
int averageVal = 0;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setRetries(15,15); 
  radio.openWritingPipe(rxAddr);
  radio.stopListening();

}

void loop() {
  int val = analogRead(A0);
  const char detected[] = "d";
  const char notDetected[] = "n";
  
  if(samples > loopVal){
    total += val;
    loopVal += 1;
  }else{
    
    averageVal = total / samples;  
    if(averageVal > thresh){
      radio.write(&detected, sizeof(detected));  
      Serial.println("Detected");
    }else{
      radio.write(&notDetected, sizeof(notDetected));        
      Serial.println("Not Detected");  
    }

    total = 0;
    loopVal = 0;
  }



//  if(val <= 200){
//      radio.write(&notDetected, sizeof(notDetected));  
//      Serial.println("Not Detected");
//  }else{
//      radio.write(&detected, sizeof(detected));  
//      Serial.println("Detected");
//  }


}
