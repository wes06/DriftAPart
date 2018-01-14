/*
  Interrupt_SPI.ino
  Brian R Taylor
  brian.taylor@bolderflight.com

  Copyright (c) 2017 Bolder Flight Systems

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software
  and associated documentation files (the "Software"), to deal in the Software without restriction,
  including without limitation the rights to use, copy, modify, merge, publish, distribute,
  sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all copies or
  substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
  BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "MPU9250.h"

// an MPU9250 object with the MPU-9250 sensor on SPI bus 0 and chip select pin 10
//
//
//

MPU9250 IMU(SPI, 10);
int status;
bool calibrating = false;
float accelXOffset;
float accelYOffset;
float accelZOffset;
int calSampleCount = 0;
int calSampleToCount = 1000;
void setup() {
  // serial to display data
  Serial.begin(2000000);
  while (!Serial) {}

  // start communication with IMU
  status = IMU.begin();
  if (status < 0) {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while (1) {}
  }
  // setting DLPF bandwidth to 184 Hz
  IMU.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_184HZ);

  IMU.setAccelRange(MPU9250::ACCEL_RANGE_2G);


  // setting SRD to 19 for a 50 Hz update rate
  // SAMPLE_RATE= Internal_Sample_Rate / (1 + SMPLRT_DIV)
  IMU.setSrd(9);
  // enabling the data ready interrupt
  IMU.enableDataReadyInterrupt();
  // attaching the interrupt to microcontroller pin 1
  pinMode(9, INPUT);
  attachInterrupt(9, getIMU, RISING);
  Serial.println("Setup done");

  Serial.println("Calibrating accel...");
  calibrating = true;



}

void loop() {
  if (calSampleCount > calSampleToCount && calibrating) {
    calibrating = false;
    accelXOffset = accelXOffset / float(calSampleToCount);
    accelYOffset = accelYOffset / float(calSampleToCount);
    accelZOffset = accelZOffset / float(calSampleToCount);
  }

}

long lastMicros = 0;

void getIMU() {
  // read the sensor
  IMU.readSensor();
  // display the data
  if (!calibrating) {
    Serial.print(micros() - lastMicros);
    //Serial.print("\t");
    //Serial.print(IMU.getAccelX_mss() - accelXOffset, 20);
    //Serial.print("\t");
    //Serial.print(IMU.getAccelY_mss() - accelYOffset, 20);
    //Serial.print("\t");
    //Serial.print(IMU.getAccelZ_mss() - accelZOffset, 20);
    //Serial.print("\t");
    Serial.print("\n");
    lastMicros = micros();
  }
  else {
    accelXOffset += 1;//IMU.getAccelX_mss();
    accelYOffset += IMU.getAccelY_mss();
    accelZOffset += IMU.getAccelZ_mss();
    calSampleCount ++;
    Serial.print("Calibrating accel: ");
    Serial.print(float(calSampleCount-1)/float(calSampleToCount)*100,1);
    Serial.print(" % done. \n");
  }

}
