#include <Average.h>
#include <AH_Pololu.h>

//*******************************************GLOBAL DECLARATION******************************************/

//Global variables infrared sensor
Average<int> baselineValues(5);
int baselineValue=0;
Average<int> rollingValues(5);
int rollingValue=0;

boolean positionVonRechtsAnfahren = false;
String orientation;
//Global stepper variables

//AH_Pololu(int RES, int DIR, int STEP, int MS1, int MS2, int MS3, int SLEEP, int ENABLE, int RESET);
AH_Pololu stepper(200,13,12,8,7,6,11,9,10);



//*******************************************INIT*******************************************************/

void setup(){
  // Set Baud rate
  Serial.begin(9600);
  
  // Infrared Initialisation
  InitInfraredSensor();
  
  // Stepper Enginer Initialisation
  InitStepper();
  
}

// Reads 10 Values from the Infrared Sensor to set the relative distance
void InitInfraredSensor(){
  for(int i=0; i<10;i++){
      baselineValues.push(analogRead(3));
      delay(250);
  }
  baselineValue = baselineValues.mean();
}

// Initialises the stepper
void InitStepper(){
  stepper.sleepON(); 

  stepper.resetDriver();                
  delay(1000);

  stepper.enableDriver();
  delay(1000); 
  
 // 0 -> Full Step 
 // 1 -> 1/2 microstepping
 // 2 -> 1/4 microstepping
 // 3 -> 1/8 microstepping
 // 4 -> 1/16 microstepping
  stepper.setMicrostepping(0); 
  delay(1000);

  stepper.setSpeedRPM(120);
  delay(1000);

  stepper.sleepOFF();                    
  delay(1000);  
}

//***********************************************MAIN***************************************************/

void loop(){
  readInfraredSensor();
  moveStepper(120, "LEFT");
}

// Moves stepper in direction with steps
void moveStepper(int steps, String dir){
  if(dir == "LEFT"){
    stepper.move(steps, BACKWARD);
  }
  else {
    stepper.move(steps, FORWARD);
  }
  
}


// Reads the current distance from infrared sensor and returns true if basket has been detected
boolean readInfraredSensor(){
  int analogValue = analogRead(3);
  
  if(abs(baselineValue - analogValue) < 40)
  {
    rollingValue = rollingValues.rolling(analogValue);
    Serial.println(rollingValue);    
    int difference = baselineValue - rollingValue;    
    if(difference >= 31 && !positionVonRechtsAnfahren)
    {
      Serial.println("DE CHÜBEL ISCH DA!!!!");
      return true;
    }
    if(difference >= 16 && positionVonRechtsAnfahren)
    {
      Serial.println("DE CHÜBEL ISCH DA!!!!");
      return true;
    }
  }
  delay(250);
  return false;
}
//******************************************************************************************************/
