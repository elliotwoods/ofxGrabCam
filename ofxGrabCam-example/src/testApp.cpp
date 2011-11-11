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
	
	ofBackground(100, 100, 100);
	ofEnableSmoothing();
	
	// REMEMBER! you have to init the camera
	// in order for it to register events.
	// I'd prefer to put this in the constructor (i.e. automatic)
	// but since it's non-standard practise it's likely to break later
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
	ofSetColor(255,100,100);
	drawGrid(5.0f, 10.0f);
	for (; p != positions.end(); p++, c++)
	{
		ofSetColor(*c);
		ofSphere(p->x, p->y, p->z, 1);
	}
	camera.end();
	//
	////
	
	ofSetColor(0,0,0);
	
	int row = 10;
	ofDrawBitmapString("ofxGrabCam", 10, row+=10);
	ofDrawBitmapString("Drag with left mouse to orbit", 10, row+=10);
	ofDrawBitmapString("Drag with right mouse to zoom", 10, row+=10);
	ofDrawBitmapString("Hold 'h' and drag with left mouse to pan", 10, row+=10);
	ofDrawBitmapString("Press 'r' to reset", 10, row+=10);

	row+=10;
	
	ofDrawBitmapString("This example", 10, row+=10);
	ofDrawBitmapString("Press 'c' to toggleCursorDraw", 10, row+=10);
	ofDrawBitmapString("Press 'u' to  toggleFixUpwards", 10, row+=10);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	if (key=='c')
		camera.toggleCursorDraw();
	
	if (key=='u')
		camera.toggleFixUpwards();
	
}

//--------------------------------------------------------------
void testApp::drawGrid(float scale, float ticks, bool labels, bool x, bool y, bool z) {
	
	ofColor c = ofGetStyle().color;
	
	ofPushStyle();
	
	if (x) {
		c.setHue(0.0f);
		ofSetColor(c);
		drawGridPlane(scale, ticks, labels);
	}
	if (y) {
		c.setHue(255.0f / 3.0f);
		ofSetColor(c);
		ofPushMatrix();
		ofRotate(90, 0, 0, 1);
		drawGridPlane(scale, ticks, labels);
		ofPopMatrix();
	}
	if (z) {
		c.setHue(255.0f * 2.0f / 3.0f);
		ofSetColor(c);
		ofPushMatrix();
		ofRotate(90, 0, 1, 0);
		drawGridPlane(scale, ticks, labels);
		ofPopMatrix();
	}
	
	ofPopStyle();
}


//--------------------------------------------------------------
void testApp::drawGridPlane(float scale, float ticks, bool labels) {

	float minor = scale / ticks;
	float major =  minor * 2.0f;
	
	ofPushStyle();
	for (int iDimension=0; iDimension<2; iDimension++)
	{
		for (float yz=-scale; yz<=scale; yz+= minor)
		{
			//major major
			if (fabs(yz) == scale || yz == 0)
				ofSetLineWidth(4);
			
			//major
			else if (yz / major == floor(yz / major) )
				ofSetLineWidth(2);
			
			//minor
			else
				ofSetLineWidth(1);
			if (iDimension==0)
				ofLine(0, yz, -scale, 0, yz, scale);
			else
				ofLine(0, -scale, yz, 0, scale, yz);
		}
	}
	ofPopStyle();
	
	if (labels) {
		//draw numbers on axes
		ofPushStyle();
		ofSetColor(255, 255, 255);
		
		ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
		for (float yz = -scale; yz<=scale; yz+=minor)
		{
			ofDrawBitmapString(ofToString(yz, 0), 0, yz, 0);
			ofDrawBitmapString(ofToString(yz, 0), 0, 0, yz);		
		}
		ofPopStyle();
	}

}


