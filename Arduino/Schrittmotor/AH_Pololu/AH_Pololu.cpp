
/*************************************************************************
**  Device: Pololu A4988/A4983 stepper driver                          	**
**  File:   AH_Pololu.h - Stepper motor Driver			    	**
**			  		     				**
**  Created by A. Hinkel 2012-05-16                                 	**
**  http://www.alhin.de/arduino/index.php?n=48   		   	**
**									**
**  Released into the public domain.  		                    	**
**                                                                  	**
*************************************************************************/


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "AH_Pololu.h"

//*******************************************************************
//*** INIT ****
//*******************************************************************
    
        
AH_Pololu::AH_Pololu(int RES, int DIR, int STEP)
{ 
  this->INIT_MODE = 0;

  this->MOTOR_RESOLUTION = RES;
  this->DIR_pin = DIR;
  this->STEP_pin = STEP;

  pinMode(this->DIR_pin, OUTPUT);
  pinMode(this->STEP_pin, OUTPUT);


  this->STEPPING_FACTOR = 1;
  setSpeedRPM(60);   		// default velocity
  
}

//*******************************************************************
AH_Pololu::AH_Pololu(int RES,int DIR,int STEP,int MS1,int MS2,int MS3,int SLEEP)
{
  this->INIT_MODE = 1;

  this->MOTOR_RESOLUTION = RES;
  this->DIR_pin = DIR;
  this->STEP_pin = STEP;
  this->MS1_pin = MS1;
  this->MS2_pin = MS2;
  this->MS3_pin = MS3;
  this->SLP_pin = SLEEP;

  pinMode(this->DIR_pin,    OUTPUT);
  pinMode(this->STEP_pin,   OUTPUT);
  pinMode(this->MS1_pin,    OUTPUT);
  pinMode(this->MS2_pin,    OUTPUT);
  pinMode(this->MS3_pin,    OUTPUT);
  pinMode(this->SLP_pin,    OUTPUT);


  setMicrostepping(0);   	// microstep default setting
  setSpeedRPM(60);   		// default velocity
  
}

//*******************************************************************
AH_Pololu::AH_Pololu(int RES,int DIR,int STEP,int MS1,int MS2,int MS3,int SLEEP,int ENABLE,int RESET)
{
  this->INIT_MODE = 2;

  this->MOTOR_RESOLUTION = RES;
  this->DIR_pin = DIR;
  this->STEP_pin = STEP;
  this->MS1_pin = MS1;
  this->MS2_pin = MS2;
  this->MS3_pin = MS3;
  this->SLP_pin = SLEEP;
  this->ENBL_pin = ENABLE;
  this->RST_pin = RESET;

  pinMode(this->DIR_pin,    OUTPUT);
  pinMode(this->STEP_pin,   OUTPUT);
  pinMode(this->MS1_pin,    OUTPUT);
  pinMode(this->MS2_pin,    OUTPUT);
  pinMode(this->MS3_pin,    OUTPUT);
  pinMode(this->SLP_pin,    OUTPUT);
  pinMode(this->ENBL_pin, OUTPUT);
  pinMode(this->RST_pin,    OUTPUT);


  setMicrostepping(0);   	// microstep default setting
  setSpeedRPM(60);   		// default velocity

  
}

//*******************************************************************
//*******************************************************************

String AH_Pololu::getVersion()
{
   return "Version: 2012-05-16";
}

//*******************************************************************
//*******************************************************************

void AH_Pololu::resetDriver(){

  if (this->INIT_MODE>1){
    digitalWrite(this->RST_pin, LOW);
    delay(1);
    digitalWrite(this->RST_pin, HIGH);
    delay(1);
  }

}

//*******************************************************************
//*******************************************************************

void AH_Pololu::setSpeedRPM(int RPM)
{
  long DIV = (long) RPM*this->MOTOR_RESOLUTION*this->STEPPING_FACTOR;  
  this->STEP_DELAY = (long) 60L*1000000L/DIV;

  this->LAST_RPM_SPEED = RPM;
  this->LAST_HZ_SPEED = 0;

  if (this->STEP_DELAY<0){
    this->STEP_DELAY = 0;
  }
}


//*******************************************************************

void AH_Pololu::setSpeedHz(int FREQ)
{
  this->STEP_DELAY = 1000000/FREQ;

  this->LAST_HZ_SPEED = FREQ;
  this->LAST_RPM_SPEED = 0;

}


//*******************************************************************

void AH_Pololu::setSpeedMax()
{
  this->LAST_RPM_SPEED = 0;
  this->LAST_HZ_SPEED = 0;

  this->STEP_DELAY = 0;

}

//*******************************************************************
//*******************************************************************

void AH_Pololu::setDirection(boolean DIRECTION){

  digitalWrite(this->DIR_pin, DIRECTION);
  delayMicroseconds(100);

}


//*******************************************************************
//*******************************************************************

void AH_Pololu::enableDriver(){

  setEnable(LOW);
}

//*********************************************

void AH_Pololu::disableDriver(){

  setEnable(HIGH);
}

//*********************************************

void AH_Pololu::setEnable(boolean VALUE){

  if (this->INIT_MODE>0){
    digitalWrite(this->ENBL_pin, VALUE);
    delayMicroseconds(100);
  }
}


//*******************************************************************
//*******************************************************************

void AH_Pololu::sleepON(){

   setSleep(LOW);
}

//********************************************

void AH_Pololu::sleepOFF(){

   setSleep(HIGH);
}

//********************************************
void AH_Pololu::setSleep(boolean VALUE){

  if (this->INIT_MODE>0){
    digitalWrite(this->SLP_pin, VALUE);
    delayMicroseconds(100);

  }

}


//*******************************************************************
//*******************************************************************

void AH_Pololu::setMicrostepping(int MODE){

  int LAST_STEPPING_FACTOR = this->STEPPING_FACTOR;  
 
  switch (MODE) {
    case 0:  					// 1/1  microstep
      digitalWrite(this->MS1_pin, LOW);
      digitalWrite(this->MS2_pin, LOW);
      digitalWrite(this->MS3_pin, LOW);
      this->STEPPING_FACTOR = 1;
      break;
    case 1:					// 1/2  microstep
      digitalWrite(this->MS1_pin, HIGH);
      digitalWrite(this->MS2_pin, LOW);
      digitalWrite(this->MS3_pin, LOW);
      this->STEPPING_FACTOR = 2;
      break;
    case 2:					// 1/4  microstep
      digitalWrite(this->MS1_pin, LOW);
      digitalWrite(this->MS2_pin, HIGH);
      digitalWrite(this->MS3_pin, LOW);
      this->STEPPING_FACTOR = 4;
      break;
    case 3:					// 1/8  microstep
      digitalWrite(this->MS1_pin, HIGH);
      digitalWrite(this->MS2_pin, HIGH);
      digitalWrite(this->MS3_pin, LOW);
      this->STEPPING_FACTOR = 8;
      break;
    case 4:					// 1/16 microstep
      digitalWrite(this->MS1_pin, HIGH);
      digitalWrite(this->MS2_pin, HIGH);
      digitalWrite(this->MS3_pin, HIGH);
      this->STEPPING_FACTOR = 16;
      break;
    default: 					// 1/16 default setting
      digitalWrite(this->MS1_pin, HIGH);
      digitalWrite(this->MS2_pin, HIGH);
      digitalWrite(this->MS3_pin, HIGH);
      this->STEPPING_FACTOR = 16;  }
   
  if (this->LAST_RPM_SPEED){
    setSpeedRPM(this->LAST_RPM_SPEED);
  }

}


//*******************************************************************
//*******************************************************************

void AH_Pololu::move(long NUMBER_OF_STEPS)
{  
  long STEPS = abs(NUMBER_OF_STEPS);  

  if (NUMBER_OF_STEPS > 0) {setDirection(FORWARD); }
  if (NUMBER_OF_STEPS < 0) {setDirection(BACKWARD);}    

  while(STEPS > 0) {
   if (micros() - this->LAST_STEP_TIME >= this->STEP_DELAY) {
      this->LAST_STEP_TIME = micros();
      STEPS--;
      stepMotor();
    }
  }

}


//*******************************************************************
//*******************************************************************

void AH_Pololu::move(long NUMBER_OF_STEPS, boolean DIRECTION)
{  
  long STEPS = abs(NUMBER_OF_STEPS);   
  setDirection(DIRECTION);

  while(STEPS > 0) {
    if (micros() - this->LAST_STEP_TIME >= this->STEP_DELAY) {
      this->LAST_STEP_TIME = micros();
      STEPS--;
      stepMotor();
    }
  }
}


//*******************************************************************
//*******************************************************************

void AH_Pololu::rotate(float DEGREES){

  int steps_to_move = (int)(DEGREES*(this->MOTOR_RESOLUTION)*(this->STEPPING_FACTOR)/360);
  move(steps_to_move);

}


//*******************************************************************
//*******************************************************************

void AH_Pololu::revolve(float TIMES){

  int steps_to_move = (int)(TIMES*(this->MOTOR_RESOLUTION)*(this->STEPPING_FACTOR)); 
  move(steps_to_move);

}

//*******************************************************************
//*******************************************************************

void AH_Pololu::stepMotor()
{
    digitalWrite(this->STEP_pin, HIGH);
    delayMicroseconds(100);
    digitalWrite(this->STEP_pin, LOW);
    delayMicroseconds(100);
}


//*******************************************************************
/*

long AH_Pololu::getMaxSpeedRPM()
{
  return (60*1000000)/(this->MOTOR_RESOLUTION*this->STEPPING_FACTOR);
}


//*******************************************************************

long AH_Pololu::getMaxSpeedHz()
{
  return 1000000;
}

*/ 