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
	polargraph.setAngularVel(100);
	polargraph.targetTime = 5;
	//polargraph.setTargetPosition(originX+coords[0][0], originY+coords[0][1]);
	polargraph.setTargetPosition(originX, originY-100);
	polargraph.computeMovementMetrics();
}

void loop() {
	/*
	IDEA:
	Copy the coordinates into a container within the polargraph object.
	*/
	
	//Serial.println(polargraph.isTargetReached());
	//polargraph.setTargetPosition(originX+coords[0][track], originY+coords[1][track]);
	/*if((polargraph.isTargetReached() || track == 1) && track < coordLen) {
		track++;
		polargraph.targetTime = 5;
		polargraph.setTargetPosition(originX+coords[track][0], originY+coords[track][1]);
		polargraph.computeMovementMetrics();
	} */
	
	/*
	if(polargraph.isTargetReached()) {
		track++;
		polargraph.targetTime = 5;
		polargraph.setTargetPosition(originX+coords[track][0], originY+coords[track][1]);
		polargraph.computeMovementMetrics();
		//Serial.println("Target Reached!");
	} */
	
	polargraph.updatePolargraph();
	//polargraph.setTargetPosition(200, 200);
}
