#include "GPController.h"

GPController polargraph;

const int coordLen = 5;
float coords[coordLen][2] = {
		{-100, 0},
		{-100, -100},
		{0, -100},
		{0, 0},
		{0, 0}
		};
		
int track = 0;
float originX = 830.0/2.0;
float originY = 582.2370651203855;

void setup() {
	polargraph.initPolargraph();
	//polargraph.setTargetPosition(100-30, 100-25);
	polargraph.setAngularVel(100);
	//polargraph.setTargetPosition(originX, originY);
	//polargraph.computeMovementMetrics();
	
	//polargraph.setPosition();
	
	//polargraph.moveGondolaDown(100);
	//polargraph.moveGondolaRight(100);
	//polargraph.moveGondolaUp(100);
	//polargraph.moveGondolaLeft(100);
}

void loop() {
	/*
	IDEA:
	Copy the coordinates into a container within the polargraph object.
	*/
	
	//Serial.println(polargraph.isTargetReached());
	//polargraph.setTargetPosition(originX+coords[0][track], originY+coords[1][track]);
	if((polargraph.isTargetReached() || track == 0) && track < coordLen) {
		track++;
		polargraph.targetTime = 5;
		polargraph.setTargetPosition(originX+coords[track][0], originY+coords[track][1]);
		polargraph.computeMovementMetrics();
	}
	polargraph.updatePolargraph();
	//polargraph.setTargetPosition(200, 200);
}
