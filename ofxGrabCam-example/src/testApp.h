#pragma once

#include "ofMain.h"

#include "ofxGrabCam.h"

class testApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();
	void drawScene();

	void keyPressed  (int key);

	ofxGrabCam camera;
	
	struct Sphere {
		ofVec3f position;
		ofColor color;
	};
	vector<Sphere> spheres;
	
	ofMatrix4x4 savedPose;
};
