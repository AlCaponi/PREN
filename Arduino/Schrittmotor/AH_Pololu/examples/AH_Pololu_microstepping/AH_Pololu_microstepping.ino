/********************************************************
** Download from:                                      **
** http://www.alhin.de/arduino/index.php?n=48          **
********************************************************/

#include <AH_Pololu.h>

//AH_Pololu(int RES, int DIR, int STEP, int MS1, int MS2, int MS3, int SLEEP);
AH_Pololu stepper(200,2,3,4,5,6,7);    // init w/o "enable" and "reset" functions

int time1;
int time2;

void setup() {               
  stepper.sleepOFF(); 
  Serial.begin(57600);
  Serial.println(stepper.getVersion());
}

void loop() {
  
  stepper.setSpeedRPM(60);                   // set speed in RPM, rotations per minute
  Serial.println();  
  Serial.println("Set 60 RPM speed");

  Serial.println("Sixteen Step 360 deg rotation");
//  stepper.setMicrostepping(4);  
  stepper.setMicrostepping(SIXTEEN_STEPMODE);  
  time1 = millis();
  stepper.rotate(360);
  time2 = millis();  
  Serial.print(time2-time1);
  Serial.println(" microsec");  
  delay(500); 
  
  Serial.println("Eight Step 360 deg rotation");
//  stepper.setMicrostepping(3);  
  stepper.setMicrostepping(EIGHT_STEPMODE);  
  time1 = millis();
  stepper.rotate(360);
  time2 = millis();  
  Serial.print(time2-time1);
  Serial.println(" microsec");  
  delay(500);    
  
  Serial.println("Quarter Step 360 deg rotation");
//  stepper.setMicrostepping(2);  
  stepper.setMicrostepping(QUARTER_STEPMODE);  
  time1 = millis();
  stepper.rotate(360);
  time2 = millis();  
  Serial.print(time2-time1);
  Serial.println(" microsec");  
  delay(500);  

  Serial.println("Half Step 360 deg rotation");
//  stepper.setMicrostepping(1);  
  stepper.setMicrostepping(HALF_STEPMODE);  
  time1 = millis();
  stepper.rotate(360);
  time2 = millis();  
  Serial.print(time2-time1);
  Serial.println(" microsec");  
  delay(500);
  
  Serial.println("Full Step 360 deg rotation");
//  stepper.setMicrostepping(0);  
  stepper.setMicrostepping(FULL_STEPMODE);  
  time1 = millis();
  stepper.rotate(360);
  time2 = millis();  
  Serial.print(time2-time1);
  Serial.println(" microsec");
  delay(500);

 
  

//*******************************************************************

  stepper.setSpeedHz(200);                   //Set speed in Hz
  Serial.println();
  Serial.println("Set 200 Hz speed");
  
  Serial.println("Full Step 360 deg rotation");
  stepper.setMicrostepping(0);  
  time1 = millis();
  stepper.rotate(360);
  time2 = millis();  
  Serial.print(time2-time1);
  Serial.println(" microsec");  
  delay(500);

  Serial.println("Half Step 360 deg rotation");
  stepper.setMicrostepping(1);  
  time1 = millis();
  stepper.rotate(360);
  time2 = millis();  
  Serial.print(time2-time1);
  Serial.println(" microsec");  
  delay(500);  

  Serial.println("Quarter Step 360 deg rotation");
  stepper.setMicrostepping(2);  
  time1 = millis();
  stepper.rotate(360);
  time2 = millis();  
  Serial.print(time2-time1);
  Serial.println(" microsec");  
  delay(500);  

  Serial.println("Eight Step 360 deg rotation");
  stepper.setMicrostepping(3);  
  time1 = millis();
  stepper.rotate(360);
  time2 = millis();  
  Serial.print(time2-time1);
  Serial.println(" microsec");  
  delay(500);  
  
  Serial.println("Sixteen Step 360 deg rotation");
  stepper.setMicrostepping(4);  
  time1 = millis();
  stepper.rotate(360);
  time2 = millis();  
  Serial.print(time2-time1);
  Serial.println(" microsec");  
  delay(500);  
}


