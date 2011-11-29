//
//  ofxGrabCam.cpp
//  ofxSketchupCamera
//
//  Created by Elliot Woods on 10/11/2011.
//  Copyright (c) 2011 Kimchi and Chips. All rights reserved.
//

#include "ofxGrabCam.h"
//--------------------------
vector<ofxInteractiveNode*> ofxGrabCam::nodes = vector<ofxInteractiveNode*>();

//--------------------------
ofxGrabCam::ofxGrabCam() : initialised(true), mouseDown(false), handDown(false), altDown(false), pickCursorFlag(false), drawCursor(false), drawCursorSize(0.1), fixUpwards(true) {
	init();
}

//--------------------------
ofxGrabCam::~ofxGrabCam() {
	//removing events actually seems to upset something
	//removeListeners();
}

//--------------------------
void ofxGrabCam::init() {
	ofCamera::setNearClip(0.5);
	
	addListeners();
	reset();
}

//--------------------------
void ofxGrabCam::begin(ofRectangle viewport) {
	glEnable(GL_DEPTH_TEST);	
	viewportRect = viewport;
	ofCamera::begin(viewport);
}

//--------------------------
void ofxGrabCam::end() {
	if (pickCursorFlag || !mouseDown) {
		findCursor();
		pickCursorFlag = false;
	}
	
	// this has to happen after all drawing + findCursor()
	// but before camera.end()
	if (drawCursor) {
		ofPushStyle();
		ofSetColor(0, 0, 0);
		ofSphere(mouseW.x, mouseW.y, mouseW.z, drawCursorSize);
		ofPopStyle();
	}
	
	ofCamera::end();
	glDisable(GL_DEPTH_TEST);
}

//--------------------------
void ofxGrabCam::reset() {
	ofCamera::resetTransform();
}

//--------------------------
void ofxGrabCam::setCursorDraw(bool enabled, float size) {
	this->drawCursor = enabled;
	this->drawCursorSize = size;
}

//--------------------------
void ofxGrabCam::toggleCursorDraw() {
	this->drawCursor ^= true;
}

//--------------------------
void ofxGrabCam::setFixUpwards(bool enabled) {
	fixUpwards = enabled;
}

//--------------------------
void ofxGrabCam::toggleFixUpwards() {
	fixUpwards ^= true;
}

//--------------------------
void ofxGrabCam::addInteractiveNode(ofxInteractiveNode &node) {
	nodes.push_back(&node);
}

//--------------------------
void ofxGrabCam::removeInteractiveNode(ofxInteractiveNode &node) {
	vector<ofxInteractiveNode*>::iterator it;
	for (it = nodes.begin(); it != nodes.end(); it++)
		if (*it == &node)
			nodes.erase(it);
	
	nodes.push_back(&node);
}

//--------------------------
void ofxGrabCam::addListeners() {
	ofAddListener(ofEvents.update, this, &ofxGrabCam::update);
    ofAddListener(ofEvents.mouseMoved, this, &ofxGrabCam::mouseMoved);
    ofAddListener(ofEvents.mousePressed, this, &ofxGrabCam::mousePressed);
    ofAddListener(ofEvents.mouseReleased, this, &ofxGrabCam::mouseReleased);
    ofAddListener(ofEvents.mouseDragged, this, &ofxGrabCam::mouseDragged);
    ofAddListener(ofEvents.keyPressed, this, &ofxGrabCam::keyPressed);
    ofAddListener(ofEvents.keyReleased, this, &ofxGrabCam::keyReleased);

	initialised = true;
}

//--------------------------
void ofxGrabCam::removeListeners() {
	if (!initialised)
		return;
	
	ofRemoveListener(ofEvents.update, this, &ofxGrabCam::update);
    ofRemoveListener(ofEvents.mouseMoved, this, &ofxGrabCam::mouseMoved);
    ofRemoveListener(ofEvents.mousePressed, this, &ofxGrabCam::mousePressed);
    ofRemoveListener(ofEvents.mouseReleased, this, &ofxGrabCam::mouseReleased);
    ofRemoveListener(ofEvents.mouseDragged, this, &ofxGrabCam::mouseDragged);
    ofRemoveListener(ofEvents.keyPressed, this, &ofxGrabCam::keyPressed);
	ofRemoveListener(ofEvents.keyReleased, this, &ofxGrabCam::keyReleased);
	
	initialised = false;
}

//--------------------------
void ofxGrabCam::update(ofEventArgs &args) {
	
}

//--------------------------
void ofxGrabCam::mouseMoved(ofMouseEventArgs &args) {
	mouseP.x = args.x;
	mouseP.y = args.y;
}

//--------------------------
void ofxGrabCam::mousePressed(ofMouseEventArgs &args) {
	mouseP.x = args.x;
	mouseP.y = args.y;
	
	if (viewportRect.inside(args.x, args.y)) {
		if (!mouseDown)
			pickCursorFlag = true;
		mouseDown = true;
	} else {
		mouseDown = false;
	}
}

//--------------------------
void ofxGrabCam::mouseReleased(ofMouseEventArgs &args) {
	mouseDown = false;
}

//--------------------------
void ofxGrabCam::mouseDragged(ofMouseEventArgs &args) {
	float dx = (args.x - mouseP.x) / ofGetViewportWidth();
	float dy = (args.y - mouseP.y) / ofGetViewportHeight();
	
	mouseP.x = args.x;
	mouseP.y = args.y;
	
	if (mouseP.z == 1.0f)
		return;
	
	ofVec3f p = ofCamera::getPosition();
	ofVec3f uy = ofCamera::getUpDir();
	ofVec3f ux = ofCamera::getSideDir();
	float ar = float(ofGetViewportWidth()) / float(ofGetViewportHeight());
	
	if (handDown) {
		//pan
		float d = (p - mouseW).length();
		//ofCamera::getFov() doesn't exist!!
		ofCamera::move(dx * -ux * d * ar);
		ofCamera::move(dy * uy * d);
	} else {
		if (args.button==0 && !altDown) {
			//orbit
			rotation.makeRotate(dx * 90 * ar, -uy, dy * 90, -ux, 0, ofVec3f(0,0,1));
			
			if (fixUpwards) {
				ofQuaternion rotToUp;
				ofVec3f sideDir = ofCamera::getSideDir() * rotation;
				rotToUp.makeRotate(sideDir, sideDir * ofVec3f(1.0f, 0, 1.0f));
				rotation *= rotToUp;
			}
			
			ofCamera::setPosition((p - mouseW) * rotation + mouseW);
			ofCamera::rotate(rotation);
		} else {
			//dolly
			ofCamera::move(2 * (mouseW - p) * dy);
		}
	}
}

//--------------------------
void ofxGrabCam::keyPressed(ofKeyEventArgs &args) {
	if (args.key == 'r')
		reset();
	
	if (args.key == 'h')
		handDown = true;
	
	if (args.key == OF_KEY_ALT)
		altDown = true;
}


//--------------------------
void ofxGrabCam::keyReleased(ofKeyEventArgs &args) {
	if (args.key == 'h')
		handDown = false;
	
	if (args.key == OF_KEY_ALT)
		altDown = false;
}


//--------------------------
void ofxGrabCam::findCursor() {
	//read z value from depth buffer at mouse coords
	glReadPixels(mouseP.x, ofGetViewportHeight()-1-mouseP.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &mouseP.z);
	
	//if we get nothing, scatter until we get something
	//we search in a spiral until we hit something
	if (mouseP.z == 1.0f) {
		float sx, sy; // search this spot in screen space
		float r, theta; // search is in polar coords
		for (int iteration=0; iteration < OFXGRABCAM_SEARCH_MAX_ITERATIONS; iteration++) {
			r = OFXGRABCAM_SEARCH_WIDTH * float(iteration) / float(OFXGRABCAM_SEARCH_MAX_ITERATIONS);
			theta = OFXGRABCAM_SEARCH_WINDINGS * 2 * PI * float(iteration) / float(OFXGRABCAM_SEARCH_MAX_ITERATIONS);
			sx = ofGetWidth() * r * cos(theta) + mouseP.x;
			sy = ofGetHeight() * r * sin(theta) + mouseP.y;
			
			if (!viewportRect.inside(sx, sy))
				continue;
			
			glReadPixels(sx, ofGetViewportHeight()-1-sy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &mouseP.z);
			
			if (mouseP.z != 1.0f)
				break;
		}
	}
	
	if (mouseP.z == 1.0f)
		return;
											   
	glGetDoublev(GL_PROJECTION_MATRIX, matP);
	glGetDoublev(GL_MODELVIEW_MATRIX, matM);
	glGetIntegerv(GL_VIEWPORT, viewport);
	
	GLdouble c[3];
	
	gluUnProject(mouseP.x, ofGetViewportHeight()-1-mouseP.y, mouseP.z, matM, matP, viewport, c, c+1, c+2);
	
	mouseW.x = c[0];
	mouseW.y = c[1];
	mouseW.z = c[2];
}