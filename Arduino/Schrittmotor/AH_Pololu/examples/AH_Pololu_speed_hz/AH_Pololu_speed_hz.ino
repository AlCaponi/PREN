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
  Serial.begin(57600);
  Serial.println(stepper.getVersion());
  stepper.setMicrostepping(1);   
}

void loop(){
  
  for (int i=100;i<2000;i=i+100){
    stepper.setSpeedHz(i);  
    Serial.print("Freq.: ");
    Serial.println(i);
    stepper.sleepOFF();
      stepper.revolve(1);
    stepper.sleepON();  
    delay(1000);
  }
  
}


