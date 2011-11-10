#pragma once

#include "ofMain.h"

#include "ofxGrabCam.h"

class testApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);

	////
	//scene
	vector<ofVec3f> positions;
	vector<ofColor> colours;

	ofxGrabCam camera;
	//
	////
};
