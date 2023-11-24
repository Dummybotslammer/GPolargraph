#include "StepperMotor.h"
#include "math.h"

/*
GPolargraph Controller code.
All length measurements are in milimeter. Everything else is computed in standard S.I. units.

NOTES:
^ goes -Y
v goes +Y
< goes -X
> goes +X
*/

#ifndef GPCONTROLLER_H
#define GPCONTROLLER_H

/*--CONSTANTS--*/
#define GP_PI 22/7

/*--CLASS--*/
class GPController {
	private:
		//Timing:
		unsigned long startTime = millis();
		unsigned long endTime = millis();
		
		//Hardware:
		StepperMotor LeftStepper;
		StepperMotor RightStepper;
		
		float gearDiameter = 12;
		
		//Data:
		int coordIndex = 0;
		float *coords;
		
		bool targetReached = false;
		float angularVel = 50;
		
		float mX = 830;
		
		float x = 0;
		float y = 0;
	
	public:
		//Variables to be relocated
		float targetTime = 0.0;
		
		//Setters
		void setCoords(float* c) {
			coords = c;
		}
		
		void setAngularVel(float vel) {
			angularVel = vel;
		}
		
		void setTargetPosition(float tx, float ty) {
			x = tx;
			y = ty;
		}
		
		void setPositionBoundaries(float tx) {
			mX = tx;
		}
		
		void setGearDiameter(float d) {
			gearDiameter = d;
		}
		
		//Getters
		bool isTargetReached() {
			return targetReached;
		}
		
		float getAngularVel() {
			return angularVel;
		}
		
		float* getTargetPosition() {
			static float pos[2];
			pos[0] = x;
			pos[1] = y;
			return pos;
		}
		
		float getPositionBoundaries() {
			/*static float pos[2];
			pos[0] = mX;
			pos[1] = mY;
			return pos;*/
			return mX;
		}
		
		float getGearDiameter() {
			return gearDiameter;
		}
	
		//Methods:
		void initPolargraph() {
			Serial.begin(9600);
			
			//Sets the configs
			LeftStepper.STEP = 2;
			LeftStepper.DIR = 3;
			LeftStepper.MODE_DEF[0] = 6;
			LeftStepper.MODE_DEF[1] = 7;
			LeftStepper.MODE_DEF[2] = 8;
			LeftStepper.mode[0] = FULL_STEP[0];
			LeftStepper.mode[1] = FULL_STEP[1];
			LeftStepper.mode[2] = FULL_STEP[2];
			LeftStepper.setPosition((long) (200.0 * 715.0/(GP_PI*gearDiameter))); //(200 * 715/(M_PI*gearDiameter)) = 3,791.666666666667

			RightStepper.STEP = 4;
			RightStepper.DIR = 5;
			RightStepper.MODE_DEF[0] = 9;
			RightStepper.MODE_DEF[1] = 10;
			RightStepper.MODE_DEF[2] = 11;
			RightStepper.mode[0] = FULL_STEP[0];
			RightStepper.mode[1] = FULL_STEP[1];
			RightStepper.mode[2] = FULL_STEP[2];
			RightStepper.setPosition(-200.0 * 715.0/(GP_PI*gearDiameter)); 
			
			//Initialization
			LeftStepper.initStepper();
			RightStepper.initStepper();
			
			LeftStepper.direction = LOW;
			RightStepper.direction = LOW;
			
			LeftStepper.speed = 100;
			RightStepper.speed = 100;
		}
		
		void moveGondolaDown(float distance) {
			//LeftStepper.speed = 100;
			//RightStepper.speed = 150;
			distance = 200 * distance/(M_PI*gearDiameter);
			
			LeftStepper.setTargetPosition(LeftStepper.getPosition() + distance);
			RightStepper.setTargetPosition(RightStepper.getPosition() - distance);
			
			unsigned long t = millis();
			LeftStepper.updateStepperPosition(t);
			RightStepper.updateStepperPosition(t);
			
			while (RightStepper.getTargetPosition() != RightStepper.getPosition() || LeftStepper.getTargetPosition() != LeftStepper.getPosition()) {
				unsigned long t = millis();
				LeftStepper.updateStepperPosition(t);
				RightStepper.updateStepperPosition(t);
			}
		}
		
		void moveGondolaUp(float distance) {
			//LeftStepper.speed = 150;
			//RightStepper.speed = 100;
			distance = 200 * distance/(M_PI*gearDiameter);
			
			LeftStepper.setTargetPosition(LeftStepper.getPosition() - distance);
			RightStepper.setTargetPosition(RightStepper.getPosition() + distance);
			
			while (RightStepper.getTargetPosition() != RightStepper.getPosition() || LeftStepper.getTargetPosition() != LeftStepper.getPosition()) {
				unsigned long t = millis();
				LeftStepper.updateStepperPosition(t);
				RightStepper.updateStepperPosition(t);
			}
		}
		
		void moveGondolaRight(float distance) {
			//LeftStepper.speed = 150;
			//RightStepper.speed = 100;
			distance = 200 * distance/(M_PI*gearDiameter);
			
			LeftStepper.setTargetPosition(LeftStepper.getPosition() + distance);
			RightStepper.setTargetPosition(RightStepper.getPosition() + distance);
			
			while (RightStepper.getTargetPosition() != RightStepper.getPosition() || LeftStepper.getTargetPosition() != LeftStepper.getPosition()) {
				unsigned long t = millis();
				LeftStepper.updateStepperPosition(t);
				RightStepper.updateStepperPosition(t);
			}
		}
		
		void moveGondolaLeft(float distance) {
			//LeftStepper.speed = 100;
			//RightStepper.speed = 150;                                                                                                                                                                                         0;
			distance = 200 * distance/(M_PI*gearDiameter);
			
			LeftStepper.setTargetPosition(LeftStepper.getPosition() - distance);
			RightStepper.setTargetPosition(RightStepper.getPosition() - distance);
			
			while (RightStepper.getTargetPosition() != RightStepper.getPosition() || LeftStepper.getTargetPosition() != LeftStepper.getPosition()) {
				unsigned long t = millis();
				LeftStepper.updateStepperPosition(t);
				RightStepper.updateStepperPosition(t);
			}
		}

		void computeMovementMetrics() {
			//Not very efficient but it works yeah
			//Converts the x and y in mm to ticks
			//float cmX = 200.0 * mX/(GP_PI*gearDiameter);
			//float cx = 200.0 * x/(GP_PI*gearDiameter);
			//float cy = 200.0 * y/(GP_PI*gearDiameter);
			
			//Left Stepper (A)
			float a1 = (LeftStepper.getPosition()/200) * (M_PI*gearDiameter);
			float a2 = sqrt(x*x + y*y);
			float va = (a2-a1)/targetTime;
			Serial.println(a1);
			Serial.println(a2);
			
			//Right Stepper (B)
			float b1 = (RightStepper.getPosition()/200) * (M_PI*gearDiameter);
			float b2 = -sqrt((mX-x)*(mX-x) + y*y);
			float vb = (b2-b1)/targetTime;
			Serial.println(b1);
			Serial.println(b2);

			va = 200.0 * va/(GP_PI*gearDiameter);
			vb = 200.0 * vb/(GP_PI*gearDiameter);
			
			a2 = 200.0 * a2/(GP_PI*gearDiameter);
			b2 = 200.0 * b2/(GP_PI*gearDiameter);
			
			Serial.println("NEW");
			Serial.println(a2);
			Serial.println(b2);
			
			LeftStepper.speed = abs((long) va);
			RightStepper.speed = abs((long) vb);	

			LeftStepper.setTargetPosition(a2);
			RightStepper.setTargetPosition(b2);
			
			/*
			if(va == 0 && vb == 0) {
				coordIndex = 0;
			}
			else { targetReached = false; }
			*/
		}

		void updatePolargraph() {
			/*
			static long oldlpos = 0;
			static long oldrpos = 0;
			static long newlpos = LeftStepper.getPosition();
			static long newrpos = RightStepper.getPosition();
			
			//Left Stepper: x and y
			float lbelt = sqrt(y*y + x*x);
			lbelt = 360 * (lbelt/M_PI*gearDiameter);
			//if(lbelt =) {}
			
			//Right Stepper: mX-x and mY-y
			float rbelt = sqrt((mY-y)*(mY-y) + (mX-x)*(mX-x));
			rbelt = 360 * (rbelt/M_PI*gearDiameter);
			rbelt = RightStepper.DIR==LOW? rbelt:-rbelt;
			
			//Checks if the targeted position is reached.
			if (newlpos == oldlpos+lbelt) { oldlpos = newlpos; LeftStepper.speed = 0; }
			else { LeftStepper.speed = angularVel; }
			
			if (newrpos == oldrpos+rbelt) { oldrpos = newrpos; RightStepper.speed = 0; }
			else { RightStepper.speed = angularVel; 
			*/
			
			//Left Stepper: x and y
			/*
			float lbelt = sqrt(y*y + x*x);
			lbelt = 200 * (lbelt/(M_PI*gearDiameter));
			Serial.println(lbelt);*/
			
			/*
			//Right Stepper: mX-x and mY-y
			float rbelt = sqrt(y*y + x*x); //sqrt((mY-y)*(mY-y) + (mX-x)*(mX-x));
			rbelt = 200 * (rbelt/(M_PsI*gearDiameter));
			Serial.println(rbelt); */
			/*
			float dY = 200 * y/(M_PI*gearDiameter);
			
			LeftStepper.setTargetPosition(dY);
			RightStepper.setTargetPosition(-dY); */
			
			/*
			float A = atan(y/(mX-x));
			float B = atan(y/x);
			
			float lbelt = x/cos(B);
			lbelt = 200 * lbelt/(M_PI*gearDiameter);
			float rbelt = (mX-x)/cos(A);
			rbelt = 200 * rbelt/(M_PI*gearDiameter);
			
			LeftStepper.setTargetPosition(lbelt);
			RightStepper.setTargetPosition(rbelt);
			*/
			
			unsigned long t = millis();
			LeftStepper.updateStepperPosition(t);
			RightStepper.updateStepperPosition(t);
			
			targetReached = false;
			if (RightStepper.getTargetPosition() == RightStepper.getPosition() && LeftStepper.getTargetPosition() == LeftStepper.getPosition()) { Serial.println("Target Reached"); targetReached = true; }
		}
};

#endif GPCONTROLLER_H