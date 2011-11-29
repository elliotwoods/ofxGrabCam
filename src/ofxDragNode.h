#pragma once
//
//  ofxDragNode.h
//  ofxGrabCam
//
//  Created by Elliot Woods on 24/11/2011.
//  Copyright (c) 2011 Kimchi and Chips. All rights reserved.
//

#include "ofxInteractiveNode.h"

class ofxDragNode : public ofxInteractiveNode {
public:
	ofxDragNode(const float hitZone=1.0f);
	
	bool hitTest(const ofVec3f &x);
	void cursorDragged(const ofVec3f &x, const ofVec3f &dx, const int button);
	
protected:
	float hitZone;
};