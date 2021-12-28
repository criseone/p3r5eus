#include <Adafruit_NeoPixel.h>

#ifdef _AVR_
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Neopixel stuff
#define PIN 7
#define NUMPIXELS 170
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//Phase 1
int brightness = 50;
int stay_on = 500;
int stay_off = 1000;

//Phase 2
int brightness1 = 100;
int stay_on1 = 250;
int stay_off1 = 250;

//Phase 3
int brightness2 = 200;
int stay_on2 = 1000;
int stay_off2 = 200;

//Phase 4
int brightness3 = 2500;


#define DELAYVAL 5 // Time (in milliseconds) to pause between pixels

// Vacuum pump stuff
int PWM_PIN = 11;
int pwmval = 0;

// State management
long phase1 = 30000;
long phase2 = 30000;
long phase3 = 30000;
long phase4 = 120000;
long phase5 = 180000;

long timestamp = 0;

int state = 1;

int pos = 0;

int sensor = 0;

void switchState(int s);

boolean isTimeUp(int duration)
{
  return millis() - timestamp > duration;
}


void setup() {

  // Neopixel stuff
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)


  //vacuum pump stuff
  Serial.begin(9600);

  pinMode(PWM_PIN, OUTPUT);

  //State management

  switchState(0);

}

void loop()
{
  if (state == 0)
  {
    pixels.clear();
    for (int i = 0; i < NUMPIXELS; i++)
    {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    }
    pixels.show();
    analogWrite(PWM_PIN, pwmval);
    Serial.println("Waiting");
    delay(200);

    if (isTimeUp(phase1))
    {
      switchState(1);
    }
  }
  if (state == 1)
  {
    pixels.clear();
    for (int i = 0;  i < NUMPIXELS; i++ ) {
      pixels.setPixelColor(i, pixels.Color(brightness, 0, 0));
    }
    pixels.show();
    delay (stay_on);
    for (int i = 0;  i < NUMPIXELS; i++ ) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    }
    pixels.show();
    delay (stay_off);
    
    analogWrite(PWM_PIN, 100);
    Serial.println("Phase 1");

    if (isTimeUp(phase2))
    {
      switchState(2);
    }
  }
    if (state == 2)
  {
    pixels.clear();
    for (int i = 0;  i < NUMPIXELS; i++ ) {
      pixels.setPixelColor(i, pixels.Color(brightness1, 0, 0));
    }
    pixels.show();
    delay (stay_on1);
    for (int i = 0;  i < NUMPIXELS; i++ ) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    }
    pixels.show();
    delay (stay_off1);
    
    analogWrite(PWM_PIN, 255);
    Serial.println("Phase 2");

    if (isTimeUp(phase3))
    {
      switchState(3);
    }
  }
  if (state == 3)
  {
    pixels.clear();
    for (int i = 0;  i < NUMPIXELS; i++ ) {
      pixels.setPixelColor(i, pixels.Color(brightness1, 0, brightness2));
    }
    pixels.show();
    delay (stay_on2);
    for (int i = 0;  i < NUMPIXELS; i++ ) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    }
    pixels.show();
    delay (stay_off2);
    
    analogWrite(PWM_PIN, 0);
    Serial.println("Phase 3");

    if (isTimeUp(phase4))
    {
      switchState(4);
    }
  }
    if (state == 4)
  {
    pixels.clear();
    for (int i = 0;  i < NUMPIXELS; i++ ) {
      pixels.setPixelColor(i, pixels.Color(0, 0, brightness3));
    }
    pixels.show();
    
    analogWrite(PWM_PIN, 0);
    Serial.println("Phase 4");

    if (isTimeUp(phase5))
    {
      switchState(0);
    }
  }
}
void switchState(int s)
{
  state = s;
  timestamp = millis();
}
