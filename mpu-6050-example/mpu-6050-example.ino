#include<Wire.h>

const int MPU_addr = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

void setup(){
  Wire.begin();

  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0); // wakes up the MPU-6050
  Wire.endTransmission(true);

  Serial.begin(9600);
}

void loop(){
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);

  Wire.requestFrom(MPU_addr, 14, true); // get 14 bytes of data from the MPU-6050
  AcX = Wire.read() <<8 | Wire.read();
  AcY = Wire.read() <<8 | Wire.read();
  AcZ = Wire.read() <<8 | Wire.read();
  Tmp = Wire.read() <<8 | Wire.read();
  GyX = Wire.read() <<8 | Wire.read();
  GyY = Wire.read() <<8 | Wire.read();
  GyZ = Wire.read() <<8 | Wire.read();

  Tmp = Tmp / 340.00 + 36.53; // convert temperature to degrees C

  Serial.print("AcX = "); Serial.print(AcX);
  Serial.print(" | AcY = "); Serial.print(AcY);
  Serial.print(" | AcZ = "); Serial.print(AcZ);
  Serial.print(" | Tmp = "); Serial.print(Tmp);
  Serial.print(" | GyX = "); Serial.print(GyX);
  Serial.print(" | GyY = "); Serial.print(GyY);
  Serial.print(" | GyZ = "); Serial.println(GyZ);

  delay(250);
}
