#pragma once

#include "ofMain.h"

#include "ofxGrabCam.h"

class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();
	void drawScene();

	void keyPressed  (int key);

	ofxGrabCam camera;
	
	struct Sphere {
		glm::vec3 position;
		ofColor color;
	};
	vector<Sphere> spheres;
	
	glm::mat4x4 savedPose;
};
