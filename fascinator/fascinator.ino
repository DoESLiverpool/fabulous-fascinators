// https://www.hackster.io/danionescu/gyroscope-fun-with-neopixel-ring-3a0b84
// https://github.com/ElectronicCats/mpu6050

#include "Wire.h"
#include <Adafruit_NeoPixel.h>


#define LED_PIN 6
#define LED_COUNT 6
#define FRAMES_PER_SECOND 60

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ400);

const int MPU_addr = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;


void setup() {
  strip.begin();
  strip.show(); // Turn OFF all pixels
  strip.setBrightness(255);

  Wire.begin();

  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0); // wakes up the MPU-6050
  Wire.endTransmission(true);
}


void loop() {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);

  Wire.requestFrom(MPU_addr, 14, true); // get 14 bytes of data from the MPU-6050
  AcX = Wire.read() <<8 | Wire.read();
  AcY = Wire.read() <<8 | Wire.read();
  AcZ = Wire.read() <<8 | Wire.read();
  // Tmp = Wire.read() <<8 | Wire.read();
  // GyX = Wire.read() <<8 | Wire.read();
  // GyY = Wire.read() <<8 | Wire.read();
  // GyZ = Wire.read() <<8 | Wire.read();

  // Tmp = Tmp / 340.00 + 36.53; // convert temperature to degrees C

  delay(150);
  setRandomLedColour( (uint8_t)(AcX >> 9), (uint8_t)(AcY >> 9), (uint8_t)(AcZ >> 9) );
}


void setRandomLedColour(uint8_t red, uint8_t green, uint8_t blue) {
  for(int i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }

  strip.setPixelColor( random(0, LED_COUNT), strip.Color(red, green, blue) );
  strip.show();
}
