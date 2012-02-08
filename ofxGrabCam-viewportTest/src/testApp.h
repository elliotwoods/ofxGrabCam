#pragma once

#include "ofMain.h"

#include "ofxGrabCam.h"

class testApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void randomiseViewport();
	
	ofxGrabCam camera;
	
	////
	//scene
	vector<ofVec3f> positions;
	vector<ofColor> colours;
	//
	////
	
	bool useViewport;
	ofRectangle viewport;
	ofMatrix4x4 savedPose;
};
