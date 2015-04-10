
/********************************************************
** Download from:                                      **
** http://www.alhin.de/arduino/index.php?n=48          **
********************************************************/

#include <AH_Pololu.h>

//AH_Pololu(int RES, int DIR, int STEP, int MS1, int MS2, int MS3, int SLEEP, int ENABLE, int RESET);
AH_Pololu stepper(200,13,12,8,7,6,11,9,10);   // init with all functions

//AH_Pololu(int RES, int DIR, int STEP, int MS1, int MS2, int MS3, int SLEEP);
//AH_Pololu stepper(200,2,3,4,5,6,7);    // init w/o "enable" and "reset" functions


void setup() {
  stepper.resetDriver();                  // reset driver
  stepper.disableDriver();                 // enable driver 
//  stepper.enableDriver();                 // enable driver 
  //stepper.setMicrostepping(0);            // 0 -> Full Step                                
                                          // 1 -> 1/2 microstepping
                                          // 2 -> 1/4 microstepping
                                          // 3 -> 1/8 microstepping
                                          // 4 -> 1/16 microstepping
  stepper.setSpeedRPM(60);               // set speed in RPM, rotations per minute
//  stepper.setSpeedHz(100);              // set speed in Hz

  stepper.sleepOFF();                     // set Sleep mode OFF
    stepper.move(3200);                   // move 1000 steps
//  stepper.move(1000,FORWARD);
  stepper.sleepON();                      // set Sleep mode ON  
  delay(1000);

  stepper.sleepOFF();                     // set Sleep mode OFF
    stepper.move(-3200);                  // move -1000 steps
//  stepper.move(1000,BACKWARD);
  stepper.sleepON();                      // set Sleep mode ON  
  delay(1000); 
  
  stepper.sleepOFF();                     // set Sleep mode OFF
    stepper.revolve(1.0);                 // revolve 2 times
  stepper.sleepON();                      // set Sleep mode ON  
  delay(1000); 
  
  stepper.sleepOFF();                     // set Sleep mode OFF
    stepper.rotate(360);                  // rotate 360°
  stepper.sleepON();                      // set Sleep mode ON  
  delay(1000); 

  stepper.disableDriver();                 // disable driver 
  stepper.move(3200);  
}

void loop() {
}


