#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	for (int i=0; i<100; i++)
	{
		positions.push_back(ofVec3f(ofRandom(-10, 10),
									ofRandom(-10, 10),
									ofRandom(-10, 10)));
		
		colours.push_back(ofColor(ofRandom(100) + 100,
								  ofRandom(100) + 100,
								  ofRandom(100) + 100));
	}
	
	// REMEMBER! you have to init the camera
	// in order for it to register events
	camera.init();
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

	vector<ofVec3f>::iterator p = positions.begin();
	vector<ofColor>::iterator c = colours.begin();
	
	////
	//draw scene
	camera.begin();
	for (; p != positions.end(); p++, c++)
	{
		ofSetColor(*c);
		ofSphere(p->x, p->y, p->z, 1);
	}
	camera.end();
	//
	////
	
	ofSetColor(0,0,0);
	ofDrawBitmapString("Press 'c' to toggle drawing the world cursor", 10, 10);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	if (key=='c')
		camera.toggleCursorDraw();
	
}
