// https://www.hackster.io/danionescu/gyroscope-fun-with-neopixel-ring-3a0b84
// https://github.com/ElectronicCats/mpu6050


#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"
#include <Adafruit_NeoPixel.h>


#define LED_PIN 6
#define LED_COUNT 6
#define FRAMES_PER_SECOND 60


MPU6050 mpu;
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ400);


bool initialization = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer
Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
volatile bool mpuInterrupt = false; // indicates whether MPU interrupt pin has gone high


void setup() {
    Serial.begin(9600);
    strip.begin();
    strip.show(); // Turn OFF all pixels
    strip.setBrightness(255);
//    initialization = initializeGyroscope();
}


void loop() {

#if 0
    if (!initialization) {
        return;
    }
    
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();
    fifoCount = mpu.getFIFOCount();
    
    if (hasFifoOverflown(mpuIntStatus, fifoCount)) {
        mpu.resetFIFO();
        return;
    }

    if (mpuIntStatus & 0x02) {
        while (fifoCount < packetSize) {
            fifoCount = mpu.getFIFOCount();
        }
        mpu.getFIFOBytes(fifoBuffer, packetSize);        
        fifoCount -= packetSize;
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

//        Serial.print("yawPitchRoll ");
//        Serial.print(ypr[0], 3);
//        Serial.print(" ");
//        Serial.print(ypr[1], 3);
//        Serial.print(" ");
//        Serial.print(ypr[2], 3);
//        Serial.println("");

        int adjustment = 200/3;
        setRandomLedColour(
            abs(ypr[0]) * adjustment,
            abs(ypr[1]) * adjustment,
            abs(ypr[2]) * adjustment
        );    
    }
#else
        setRandomLedColour(
           random(255),random(255),random(255)
           );    
#endif

    delay(100);
//    mpu.resetFIFO();
}


void setStripColour(float red, float green, float blue) {

    for(int i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(red, green, blue));
    }
    strip.show();
}

void setRandomLedColour(float red, float green, float blue) {
    for(int i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(0,0,0));
    }
    strip.setPixelColor( random(0, LED_COUNT), strip.Color(red, green, blue) );
    strip.show();
}



bool initializeGyroscope() {
    Wire.begin();
    TWBR = 24;  
    mpu.initialize();
    devStatus = mpu.dmpInitialize();
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788);
    if (devStatus != 0) {
        return false;
    }
    mpu.setDMPEnabled(true);
    attachInterrupt(0, dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();
    packetSize = mpu.dmpGetFIFOPacketSize();
    return true;
}


void dmpDataReady() {
    mpuInterrupt = true;
}


boolean hasFifoOverflown(int mpuIntStatus, int fifoCount) {
    return mpuIntStatus & 0x10 || fifoCount == 1024;
}
