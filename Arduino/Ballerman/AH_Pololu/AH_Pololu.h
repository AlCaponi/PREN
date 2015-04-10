
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


#ifndef AH_Polopu_h
#define AH_Polopu_h


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


#define FORWARD          true
#define BACKWARD         false
#define FULL_STEPMODE    0
#define HALF_STEPMODE    1
#define QUARTER_STEPMODE 2
#define EIGHT_STEPMODE   3
#define SIXTEEN_STEPMODE 4


class AH_Pololu {
  public:
    AH_Pololu(int RES,int DIR,int STEP);
    AH_Pololu(int RES,int DIR,int STEP,int MS1,int MS2,int MS3,int SLEEP);
    AH_Pololu(int RES,int DIR,int STEP,int MS1,int MS2,int MS3,int SLEEP,int ENABLE,int RESET);


    void setSpeedRPM(int RPM);
    void setSpeedHz(int FREQ);
    void setSpeedMax(); 
    void setMicrostepping(int MODE);

    void enableDriver();
    void disableDriver();
    void sleepON();
    void sleepOFF();
    void resetDriver();

    void move(long NUMBER_OF_STEPS);
    void move(long NUMBER_OF_STEPS, boolean DIRECTION);
    void rotate(float DEGREES);
    void revolve(float TIMES);

    String getVersion();


  private:
    void stepMotor();
    void setDirection(boolean DIRECTION);
    void setEnable(boolean VALUE);
    void setSleep(boolean VALUE);

    long STEP_DELAY;
    long LAST_STEP_TIME;

    int LAST_RPM_SPEED;
    int LAST_HZ_SPEED;
    int INIT_MODE;
    int STEP_NUMBER;
    int MOTOR_RESOLUTION;
    int DIRECTION;
    int STEPPING_FACTOR;

    int DIR_pin;
    int STEP_pin;
    int MS1_pin;
    int MS2_pin;
    int MS3_pin;
    int SLP_pin;
    int ENBL_pin;
    int RST_pin;
	

};

#endif

 
