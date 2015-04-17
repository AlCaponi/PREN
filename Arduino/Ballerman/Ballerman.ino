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
int initialSteps;
boolean firstLoop = true;

//Global stepper variables

//AH_Pololu(int RES, int DIR, int STEP, int MS1, int MS2, int MS3, int SLEEP, int ENABLE, int RESET);
AH_Pololu mover_stepper(200,13,12,8,7,6,11,9,10);
// TODO: Stepper PIN configuration 
AH_Pololu shooter_stepper(200,13,12,8,7,6,11,9,10);


//*******************************************INIT*******************************************************/

void setup(){
  // Set Baud rate
  Serial.begin(9600);
  Serial.println("Begin Setup");
  
  // Infrared Initialisation
  Serial.println("Begin Infrared Init");
  InitInfraredSensor();
  Serial.println("End Infrared Init");
  
  // Stepper Enginer Initialisation
  Serial.println("Begin Stepper Init");
  InitStepper(mover_stepper);
  InitStepper(shooter_stepper);
  Serial.println("End Stepper Init");
  
  // Read arguments from Raspberry PI
  Serial.println("Begin direction Init");
  String startupDirection = "";
  while (startupDirection == ""){
    if (Serial.available() > 0)  {
        startupDirection = Serial.readStringUntil('\n');
        delay(1000);
        Serial.println("Direction found" + startupDirection);
    }
  }
  orientation = startupDirection;
  Serial.println("End direction Init");
  
  // Read initial steps from Raspberry PI 
  Serial.println("Begin Step Init");
  int startupSteps = 0;
  while (startupSteps == 0){
    if (Serial.available() > 0)  {
        startupSteps = Serial.parseInt();
         Serial.println("Found Steps " + startupSteps);
        delay(1000);
    }
  }
  initialSteps = startupSteps;
  Serial.println("End Step Init");
  
  Serial.println("End Setup");
  
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
void InitStepper(AH_Pololu stepper){
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
  
  if(firstLoop){
    moveStepper(initialSteps, orientation);    
    firstLoop = false;
  }
  if(readInfraredSensor()){
    fire();
  }
  else{
    moveStepper(10, orientation);
  }
  
}

void fire(){
  // TODO: Herausfinden wie viele Schritte nötigen sind für eine Umdrehung
  int momentanSteps=0;
  int rampeSteps=5;
  for(int rampe=60; rampe<=120; rampe+= 5){
      momentanSteps+=rampeSteps;
      shooter_stepper.setSpeedRPM(rampe);
      shooter_stepper.move(rampeSteps);      // move 1000 steps
    }
  shooter_stepper.move(2000-momentanSteps);
}

// Moves stepper in direction with steps
void moveStepper(int steps, String dir){
  int momentanSteps=0; //Bereits absolvierte Schritte
  int rampeSteps=10;   // Anzahl Schritte pro Rampenfunktion
  for(int rampe=60; rampe<=120;rampe+=5){
    if(momentanSteps<=steps){ //Abgefangen ob alle Schritte bereits gemacht wurden
      momentanSteps+=rampeSteps;
      shooter_stepper.setSpeedRPM(rampe); //Geschwindikeit setzen
      if(dir=="LEFT"){
        mover_stepper.move(rampeSteps,BACKWARD);
        if((rampe==120)&&!(momentanSteps==steps){
          mover_stepper.move((steps-momentanSteps),BACKWARD);
        }
      }
      else{
        mover_stepper.move(rampeSteps,FORWARD);
        if((rampe==120)&&!(momentanSteps==steps){
          mover_stepper.move((steps-momentanSteps),FORWARD);
        }
      }
    }
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
