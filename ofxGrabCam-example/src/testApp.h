#pragma once

#include "ofMain.h"

#include "ofxGrabCam.h"

class testApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);

	ofxGrabCam camera;
	
	////
	//scene
	vector<ofVec3f> positions;
	vector<ofColor> colours;
	//
	void drawGrid(float scale = 10.0f, float ticks = 5.0f, bool labels = false, bool x = true, bool y = true, bool z = true);
	void drawGridPlane(float scale, float ticks = 5.0f, bool labels = false);
	//
	////
	
	ofMatrix4x4 savedPose;
};
