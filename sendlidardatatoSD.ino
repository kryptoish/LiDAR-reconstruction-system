#include <SoftwareSerial.h>
#include <AccelStepper.h>
#include <Adafruit_MotorShield.h>
#include <SPI.h>
#include <SD.h>
#include "TFmini.h"
#include <Stepper.h>

TFmini tfmini;
File scan_data;
Adafruit_MotorShield AFMStop(0x60);
Adafruit_StepperMotor *myStepper2 = AFMStop.getStepper(200, 2); //M4:blck, grn M3:red, blu

const int stepsPerRevolution = 200;
Stepper myStepper(stepsPerRevolution, 36, 37, 38, 39);

void forwardstep2() {
  myStepper2->onestep(FORWARD, DOUBLE);
}
void backwardstep2() {
  myStepper2->onestep(BACKWARD, DOUBLE);
}

AccelStepper stepper2(forwardstep2, backwardstep2);

int data;
const int chipSelect = 48;
bool delayRunning = false;
unsigned int delayStart;

void setup() {
  // put your setup code here, to run once:
  // delayStart = millis();   // start delay
  delayRunning = true;
  AFMStop.begin();

  myStepper.setSpeed(100);

  myStepper2->setSpeed(15);

  Serial.begin(115200);
  Serial1.begin(TFmini::DEFAULT_BAUDRATE);
  tfmini.attach(Serial1); //green to 19 white to 18

  Serial.print("Initializing SD card...");

  if (!SD.begin(48)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  scan_data = SD.open("test.txt", FILE_WRITE);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (delayRunning) {

    for (int j = 0; j < 10; j++){

      myStepper.step(stepsPerRevolution);
      myStepper.step(stepsPerRevolution);
      myStepper.step(stepsPerRevolution);
      myStepper.step(stepsPerRevolution);
      myStepper.step(stepsPerRevolution);

      for (int i = 0; i < 200; i++) {
        if (tfmini.available()) {
          data = int(tfmini.getDistance()); //turns slightly then reads  then repeat
          Serial.println(data);
          scan_data.println(data);
        }
        myStepper2->step(1,FORWARD,DOUBLE);
      }
    }

    delayRunning = false;
    scan_data.close();
    Serial.println("REMOVE SD CARD...");

    /*
    for (int j = 0;j < 10; j++){

      myStepper.step(-stepsPerRevolution);
      myStepper.step(-stepsPerRevolution);
      myStepper.step(-stepsPerRevolution);
      myStepper.step(-stepsPerRevolution);
      myStepper.step(-stepsPerRevolution);
    }  
    */
  }
}
