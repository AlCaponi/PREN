#include <AH_Pololu.h>

//AH_Pololu(int RES, int DIR, int STEP, int MS1, int MS2, int MS3, int SLEEP, int ENABLE, int RESET);
AH_Pololu stepper(200,13,12,8,7,6,11,9,10);   // init with all functions

//AH_Pololu(int RES, int DIR, int STEP, int MS1, int MS2, int MS3, int SLEEP);
//AH_Pololu stepper(200,2,3,4,5,6,7);    // init w/o "enable" and "reset" functions


void setup() {
  //digitalWrite(12, LOW);
  stepper.sleepON(); 
  Serial.begin(9600);
  Serial.println("resetDriver");
  stepper.resetDriver();                  // reset driver
  delay(1000);
  //stepper.disableDriver();                 // enable driver 
  Serial.println("disableDriver");
  stepper.enableDriver();                 // enable driver 
  Serial.println("enableDriver");
  delay(1000);
  Serial.println("Microstepping");
  stepper.setMicrostepping(0);            // 0 -> Full Step                              
  delay(1000);  
                                          // 1 -> 1/2 microstepping
                                          // 2 -> 1/4 microstepping
                                          // 3 -> 1/8 microstepping
                                          // 4 -> 1/16 microstepping
  Serial.println("setSpeed");
  stepper.setSpeedRPM(120);               // set speed in RPM, rotations per minute
  delay(1000);
//  stepper.setSpeedHz(100);              // set speed in Hz
  
  Serial.println("sleepOff");
  stepper.sleepOFF();                     // set Sleep mode OFF
  delay(1000);
    Serial.println("move +");
    stepper.move(2000);                   // move 1000 steps
    delay(1000);
    //Serial.println("move -");
    //stepper.move(-200);
    //delay(1000);
  stepper.move(1000, BACKWARD);
  Serial.println("sleepOn");
  stepper.sleepON();                      // set Sleep mode ON  
  delay(1000);
}

void loop(){
}
