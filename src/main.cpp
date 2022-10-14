#include <Arduino.h>

/*
* Code loosely based on: https://medium.com/@werneckpaiva/how-to-read-rc-receiver-signal-with-arduino-54e0447f6c3f
*/

//#define DEBUG_INPUT // Uncomment to debug input values

/*
  Input setup
*/
#define CH1 D8
#define CH2 D7
#define CH3 D6
#define CH4 D5

int deadzone = 20; // Deadzone for inputs. Between -deadzone and deadzone inputs are ignored

int minValue = -190;
int maxValue = 190;

int rangeMin = -255;
int rangeMax = 255;

/*
  Output setup
*/
#define M1A D1
#define M1B D2
#define M2A D3
#define M2B D4


// Read the number of a given channel and convert to the range provided.
// If the channel is off, return the default value
int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue){
  int ch = pulseIn(channelInput, HIGH, 30000);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

void writeOutput(int input, int deadzone, int minInput, int maxInput, uint8_t forwardPin, uint8_t backwardPin) {
  if ((input > deadzone) || (input < -deadzone))
  {
    int constrainedInput = constrain(input, minInput, maxInput); // Prevent out of bound input
    
    if (constrainedInput >= 0) // Forward
    {
      int output = map(input, 0, maxInput, 0, 255); // Calculate pwm value
      analogWrite(forwardPin, output); // Set pwm value on forward pin
      analogWrite(backwardPin, 0); // Set backward pin to 0
    }
    else // Backward
    {
      int output = map(-input, 0, maxInput, 0, 255); // Calculate pwm value
      analogWrite(backwardPin, output); // Set pwm value on backward pin
      analogWrite(forwardPin, 0); // Set forward pin to 0
    }
    
  } else { // Input is inside deadzone
    analogWrite(forwardPin, 0);
    analogWrite(backwardPin, 0);
  }
} 

void setup() {
  // put your setup code here, to run once:
  #ifdef DEBUG_INPUT
    Serial.begin(115200);
  #endif
  
  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);
  pinMode(CH3, INPUT);
  pinMode(CH4, INPUT);

  pinMode(M1A, OUTPUT);
  pinMode(M1B, OUTPUT);
  pinMode(M2A, OUTPUT);
  pinMode(M2B, OUTPUT);
}

int ch1Value, ch2Value, ch3Value, ch4Value;

void loop() {
  // put your main code here, to run repeatedly:

  // Læs inputs
  ch1Value = readChannel(CH1, rangeMin, rangeMax, 0);
  ch2Value = readChannel(CH2, rangeMin, rangeMax, 0);
  ch3Value = readChannel(CH3, rangeMin, rangeMax, 0);
  ch4Value = readChannel(CH4, rangeMin, rangeMax, 0);

  // Sæt output
  writeOutput(ch1Value, deadzone, minValue, maxValue, M1A, M1B); // Motor 1
  writeOutput(ch2Value, deadzone, minValue, maxValue, M2A, M2B); // Motor 2


  #ifdef DEBUG_INPUT
    // Debug: Print læste værdier
    Serial.print(ch1Value);
    Serial.print("\t");
    Serial.print(ch2Value);
    Serial.print("\t");
    Serial.print(ch3Value);
    Serial.print("\t");
    Serial.println(ch4Value);
  #endif



  //delay(500);
}