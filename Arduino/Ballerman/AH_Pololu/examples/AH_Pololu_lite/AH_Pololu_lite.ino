
/********************************************************
** Download from:                                      **
** http://www.alhin.de/arduino/index.php?n=48          **
********************************************************/

#include <AH_Pololu.h>

// RES -> RESOLUTION per full revolve
// DIR -> DIRECTION PIN
// STEP -> STEPPING PIN
// AH_Polopu(int RES, int DIR, int STEP);
AH_Pololu stepper(200,2,3);   // Initialisation

void setup() {
  stepper.setSpeedRPM(60);          // RPM , rotations per minute
//stepper.setSpeedHz(100);           // Hz, steps per second
}

void loop() {

  stepper.move(200);               // move 200 steps
  delay(1000);
  
  stepper.revolve(1.0);            // revolve one time
  delay(1000);
  
  stepper.rotate(360);             // rotate 360° 
  delay(1000);  
  
}


