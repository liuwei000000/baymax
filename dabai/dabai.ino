#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define PIN 9
int sensorPin = A3;    // select the input pin for the potentiometer
int sensorValue = 0;  // variable to store the value coming from the sensor
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);
//float PI = 3.1415926;

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  Serial.begin(9600);
}

void loop() {
  /*int i = random(1, 22);
  if (i > 21 - 6 / 2)
    Serial.println("l");
  else if (i < 7)
    Serial.println("B");
  else if (i < 12)
    Serial.println("G");
  else
    Serial.println("Rand");
  */
  int sum = 0;
  for (int i = 0; i < 8000; i++) {
    sensorValue = analogRead(sensorPin);    
    if (sensorValue < 0x3f0) {
      //Serial.println(sensorValue, HEX);
      sum+=1;
    }
  }
  Serial.println(sum);
  if (sum > 2)
    led(sum);
}

void led(int s) {
  int i = random(1, 22);
  if (i > 20 - s / 2)
    little();
  else if (i < 7)
    huxi_blue();
  else if (i < 12)
    huxi_green();
  else
    huxi_rand();
}

void little() {
  int t = random(1, 3);
  int time = random(5, 51);
  int f = 1;
  if (time < 30) f = random(1, 3);
  if (time < 15) f = random(2, 4);
  if (t == 1) {
        theaterChaseRainbow(time);
  } else {
      for (int i = 0; i < f; i++) {
        rainbow(time);
        delay(500);
      }
  }
}

void huxi_blue() {
  int sum = random(3, 6);
  int time = random(50, 410);
  for (int i = 0; i < sum; i++) {
    huxi(strip.Color(0, 0, 255), time);
  }
}

void huxi_green() {
  int sum = random(3, 6);
  int time = random(50, 410);
  for (int i = 0; i < sum; i++) {
    huxi(strip.Color(0, 255, 0), time);
  }
}

void huxi_rand() {
  int sum = random(3, 6);
  int time = random(50, 410);
  int r = random(3, 256);
  int g = random(3, 256);
  int b = random(3, 256);
  for (int i = 0; i < sum; i++) {
    huxi(strip.Color(r, g, b), time);
  }
}

void huxi(uint32_t c, uint16_t st)
{
  uint8_t b = c & 0xff;
  uint8_t g = (c & 0xff00) >> 8 ;
  uint8_t r = (c & 0xff0000) >> 16;  
  float s = (PI / 2) / st;
  for (float i = 0; i < PI; i += s ) {
    uint8_t bt = b * sin(i);
    uint8_t gt = g * sin(i);
    uint8_t rt = r * sin(i);
    theaterChase2(strip.Color(rt, gt, bt), 0);
  }
  theaterChase2(strip.Color(0, 0, 0), 0);
  delay(2*st + 500);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j, q;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }  
  delay(500);  
  for (q=0; q < 3; q++) {
     for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
     }
     strip.show();
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

void theaterChase2(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
     
      if (!wait)
      delay(wait);
    }
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
     
      if (!wait)
      delay(wait);
     
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        strip.show();
       
        delay(wait);
       
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

