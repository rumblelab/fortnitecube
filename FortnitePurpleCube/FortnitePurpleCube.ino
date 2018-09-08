#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
//increase this value to make Kevin less sensitive, decrease it to make him more sensitive
int accelThreshold = 2;

//Give Pin's on the trinket a purpose
int ledPIN = 1;
int sfxPin = 0;

//Tell the program, how many LED's we will use
int NUMPIXELS = 5;

//initialize variables for us to do some math with later
int prevAccelVal = 0;
int currentAccelVal = 0;
int accelDiff = 0;
int lastHitTime = 0;//variable to debounce Kevin...why do people call it debounce
int currentHitTime = 0;
int delayBetweenHits = 3000;
int timeBetweenHits = 0;

bool justFired = true;//boolean to make sure that kevin doesnt fire back to back
bool kevinIsHit = false;//boolean to control when to complete the lightning strike routine

//Define a Neopixel object "strip" that we will use in our program
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, ledPIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif

  //initialize pins
  pinMode(sfxPin , OUTPUT);
  digitalWrite(sfxPin , HIGH);
  
  // Wake up Neopixels, it's time to get to work...
  strip.begin(); 
  strip.show();
  
  //start Kevin out in a laid back mood  
  kevinIsCalm();
}

void loop() {
  
  checkForStrike();
  
  if(kevinIsHit){
  kevinIsMad();
  }
      
  else{
    kevinIsCalm(); 
  }
}

  //function to check if Kevin was hit
  void checkForStrike(){
    prevAccelVal = currentAccelVal;
    currentAccelVal = analogRead(1);
    currentAccelVal = analogRead(1);
    accelDiff = abs(currentAccelVal - prevAccelVal);
    if(accelDiff>accelThreshold){
      currentHitTime = millis();
      timeBetweenHits = currentHitTime - lastHitTime;
      if(timeBetweenHits>delayBetweenHits){//we need a delay between hits to keep funny things from happening
          kevinIsHit = true;
          lastHitTime = millis();
      }

  }

  else{
    kevinIsHit = false;
  }
}
  //take Kevin back to his happy place
void kevinIsCalm(){
  for (int i = 0 ; i<NUMPIXELS ; i++){
  strip.setPixelColor(i, 10, 10, 10);
  }
  strip.show();
}

void playAngryKevinSound(){
  digitalWrite(sfxPin , LOW);//sending the sfx pin to ground causes sound to play
}

void resetSfxPin(){
  digitalWrite(sfxPin , HIGH);
}

void kevinIsMad(){
  playAngryKevinSound();
  //gradually get a little brighter
  for (int j = 10; j < 50 ; j++){
    for (int i = 0 ; i<NUMPIXELS ; i++){
    strip.setPixelColor(i, j, j, j);
    }
    strip.show();
    delay(24);
}
  resetSfxPin();

  //fire lightning
    for (int j = 20; j < 250 ; j++){
    for (int i = 0 ; i<NUMPIXELS ; i++){
    strip.setPixelColor(i, j, j, j);
    }
    strip.show();
}

//gradually dim colors back to Kevin's happy color
for (int j = 250; j > 10 ; j--){
    for (int i = 0 ; i<NUMPIXELS ; i++){
    strip.setPixelColor(i, j, j, j);
    }
    strip.show();
    delay(1);
}

//Kevin got out his anger, now reset him
kevinIsHit = false;
}

