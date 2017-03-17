#include <Wire.h>
#include <SparkFun_APDS9960.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

/// Pins
#define APDS9960_INT    2
int REDPin = 4;    // RED pin of the LED to PWM pin 4
int GREENPin = 5;  // GREEN pin of the LED to PWM pin 5
int BLUEPin = 6;   // BLUE pin of the LED to PWM pin 6

#define PIN            9
#define NUMPIXELS      60

#define SPEAK   8

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

SparkFun_APDS9960 apds = SparkFun_APDS9960();
int isr_flag = 0;

void setup()
{
  pinMode(APDS9960_INT, INPUT);
  pinMode(REDPin, OUTPUT);
  pinMode(GREENPin, OUTPUT);
  pinMode(BLUEPin, OUTPUT);
  Serial.begin(9600);

  pixels.begin();


  // Initialize interrupt service routine
  attachInterrupt(0, interruptRoutine, FALLING);

  // Initialize APDS-9960 (configure I2C and initial values)
  if ( apds.init() ) {
    Serial.println(F("APDS-9960 initialization complete"));
  } else {
    Serial.println(F("Something went wrong during APDS-9960 init!"));
  }
  
  // Start running the APDS-9960 gesture sensor engine
  if ( apds.enableGestureSensor(true) ) {
    Serial.println(F("Gesture sensor is now running"));
  } else {
    Serial.println(F("Something went wrong during gesture sensor init!"));
  }

  
}

void loop()
{
  analogWrite(REDPin, 255);
  analogWrite(GREENPin, 255);
  analogWrite(BLUEPin, 255);


  for(int i=47;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(255,255,255));
  }
  pixels.show();
  
  if( isr_flag == 1 ) {
    detachInterrupt(0);
    handleGesture();
    isr_flag = 0;
    attachInterrupt(0, interruptRoutine, FALLING);
  }
  
}

void interruptRoutine() {
  isr_flag = 1;
}

void handleGesture() {
    if ( apds.isGestureAvailable() ) {
    switch ( apds.readGesture() ) {
      case DIR_UP:
        //yellow
        Serial.println("UP");
        analogWrite(BLUEPin, 0);
        analogWrite(GREENPin, 100);
        for(int i=47;i<NUMPIXELS;i++){
          pixels.setPixelColor(i, pixels.Color(255,100,0));
        }
        pixels.show();
        tone(SPEAK, 440, 500);
        delay(1000);
        tone(SPEAK, 1540, 500);
        delay(2000);
        analogWrite(BLUEPin, 255);
        analogWrite(GREENPin, 255);
        for(int i=47;i<NUMPIXELS;i++){
          pixels.setPixelColor(i, pixels.Color(255,255,255));
        }
        pixels.show();
        break;
      case DIR_DOWN:
        //blue
        Serial.println("DOWN");
        analogWrite(REDPin, 0);
        analogWrite(GREENPin, 0);
        for(int i=47;i<NUMPIXELS;i++){
          pixels.setPixelColor(i, pixels.Color(0,0,255));
        }
        pixels.show();
        tone(SPEAK, 1540, 500);
        delay(1000);
        tone(SPEAK, 440, 500);
        delay(2000);
        analogWrite(REDPin, 255);
        analogWrite(GREENPin, 255);
        for(int i=47;i<NUMPIXELS;i++){
          pixels.setPixelColor(i, pixels.Color(255,255,255));
        }
        pixels.show();
        break;
      case DIR_LEFT:
        //green
        Serial.println("LEFT");
        analogWrite(REDPin, 0);
        analogWrite(BLUEPin, 0);
        for(int i=47;i<NUMPIXELS;i++){
          pixels.setPixelColor(i, pixels.Color(0,255,0));
        }
        pixels.show();
        tone(SPEAK, 440, 500);
        delay(1000);
        tone(SPEAK, 1540, 500);
        delay(1000);
        tone(SPEAK, 2500, 500);
        delay(2000);
        analogWrite(REDPin, 255);
        analogWrite(BLUEPin, 255);
        for(int i=47;i<NUMPIXELS;i++){
          pixels.setPixelColor(i, pixels.Color(255,255,255));
        }
        break;
      case DIR_RIGHT:
        Serial.println("RIGHT");
        analogWrite(GREENPin, 0);
        for(int i=47;i<NUMPIXELS;i++){
          pixels.setPixelColor(i, pixels.Color(255,0,255));
        }
        pixels.show();
        tone(SPEAK, 2500, 500);
        delay(1000);
        tone(SPEAK, 1540, 500);
        delay(1000);
        tone(SPEAK, 440, 500);
        delay(2000);
        analogWrite(GREENPin, 255);
        for(int i=47;i<NUMPIXELS;i++){
          pixels.setPixelColor(i, pixels.Color(255,255,255));
        }
        break;
      case DIR_NEAR:
        Serial.println("NEAR");
        analogWrite(GREENPin, 0);
        analogWrite(BLUEPin, 0);
        for(int i=47;i<NUMPIXELS;i++){
          pixels.setPixelColor(i, pixels.Color(255,0,0));
        }
        pixels.show();
        tone(SPEAK, 440, 1000);
        delay(3000);
        analogWrite(GREENPin, 255);
        analogWrite(BLUEPin, 255);
        for(int i=47;i<NUMPIXELS;i++){
          pixels.setPixelColor(i, pixels.Color(255,255,255));
        }
        break;
      case DIR_FAR:
        Serial.println("FAR");
        analogWrite(GREENPin, 0);
        for(int i=47;i<NUMPIXELS;i++){
          pixels.setPixelColor(i, pixels.Color(244,66,185));
        }
        pixels.show();
        tone(SPEAK, 440, 500);
        delay(1000);
        tone(SPEAK, 1540, 500);
        delay(1000);
        tone(SPEAK, 440, 500);
        delay(2000);
        analogWrite(GREENPin, 255);
        for(int i=47;i<NUMPIXELS;i++){
          pixels.setPixelColor(i, pixels.Color(255,255,255));
        }
        break;
      default:
        Serial.println("NONE");
        tone(SPEAK, 300, 250);
    }
  }
}



