#include "Arduino.h"

/*
Custom writen library for controlling the 17HS4023 Stepper Motor, or any 4-pin bipolar stepper motor.
All units are S.I. standard unless otherwise stated.
Angles are all in degrees.

NOTES:
DIR LOW - CW
DIR HIGH - CCW
*/

#ifndef STEPPERMOTOR_H
#define STEPPERMOTOR_H

/*--CONSTANTS & FIXED DEFINITIONS--*/
const unsigned int FULL_STEP[3] = {0, 0, 0}; 
const unsigned int HALF_STEP[3] = {1, 0, 0};

/*--STATICS--*/
static long startT;
static long endT;
static long elapsedT;

/*--CLASS--*/
class StepperMotor {
	private:
		long position = 0;
		long targetPosition = 0;
	
	public:
		//Pins:
		unsigned int STEP = 0;
		unsigned int DIR = 0;
		unsigned int MODE_DEF[3] = {0, 0, 0};
	
		//Data:
		unsigned int direction = LOW;
		unsigned int mode[3] = {LOW, LOW, LOW};
		long speed = 0;
		
		//Constructors: TODO
		
		//Setters:
		void setPosition(long pos) {
			position = pos;
		}
		
		void setTargetPosition(long pos) {
			targetPosition = pos;
		}
		
		//Getters:
		long getPosition() {
			return position;
		}
		
		long getTargetPosition() {
			return targetPosition;
		}
		
		//Methods:
		void initStepper() {
			pinMode(STEP, OUTPUT);
			pinMode(DIR, OUTPUT);
	
			pinMode(MODE_DEF[0], OUTPUT);
			pinMode(MODE_DEF[1], OUTPUT);
			pinMode(MODE_DEF[2], OUTPUT);
	
			digitalWrite(MODE_DEF[0], mode[0]);
			digitalWrite(MODE_DEF[1], mode[1]);
			digitalWrite(MODE_DEF[2], mode[2]);
		}

		void pulseStepper() {
			digitalWrite(DIR, direction);
			digitalWrite(STEP, LOW);
			delayMicroseconds(1000);
			digitalWrite(STEP, HIGH);
			if(direction == LOW) {position++;}
			else {position--;}
		}

		void rotateAngle(float theta, unsigned long pulsedelay) {
			//((float) (2-motor.mode[0])*1/2) * ((float) (4-motor.mode[1]*3)*1/4) * ((float) (16-motor.mode[2]*15)*1/16)
			float stepAngle = 1.8 * (1 - mode[0]/2) * (1 - mode[1]*3/4) * (1 - mode[2]*15/16);
			unsigned int pulses =  (theta/stepAngle>=1 ? theta/stepAngle:1); 
	
			for(int i=0; i < pulses; i++) { 
				pulseStepper();
				delay(pulsedelay);
			}
		}

		void updateStepperPosition(unsigned long time) {
			//This function ought to be called for every 'main' loop iteration to ensure consistent functionality.
			
			if (targetPosition > position) {
				direction = LOW;
				//pulseStepper();
				//delay(5);
				if(time % (int)(1/(float)speed*1000) == 0) { pulseStepper(); }
			}
			
			else if (targetPosition < position) {
				direction = HIGH;
				//pulseStepper();
				//delay(5);
				if(time % (int)(1/(float)speed*1000) == 0) { pulseStepper(); }
			}
		}
};

#endif