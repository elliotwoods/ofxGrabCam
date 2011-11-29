//
//  ofxDragNode.cpp
//  ofxGrabCam
//
//  Created by Elliot Woods on 24/11/2011.
//  Copyright (c) 2011 Kimchi and Chips. All rights reserved.
//

#include "ofxDragNode.h"

ofxDragNode::ofxDragNode(const float hitZone) {
	this->hitZone = hitZone;
}

bool ofxDragNode::hitTest(const ofVec3f &x) {
	return (x - this->getGlobalPosition()).length() < hitZone;
}

void ofxDragNode::cursorDragged(const ofVec3f &x, const ofVec3f &dx, const int button) {
	this->move(dx);
}