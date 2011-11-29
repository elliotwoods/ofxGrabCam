#pragma once
//
//  ofxInteractiveNode.h
//  ofxGrabCam
//
//  Created by Elliot Woods on 10/11/2011.
//  Copyright (c) 2011 Kimchi and Chips. All rights reserved.
//

#include "ofMain.h"

class ofxInteractiveNode : public ofNode {
public:
	virtual bool hitTest(const ofVec3f &x) = 0;
	virtual void cursorDown(const ofVec3f &x, const int button) { };
	virtual void cursorReleased(const ofVec3f &x, const int button) { };
	virtual void cursorMoved(const ofVec3f &x, const ofVec3f &dx) { };
	virtual void cursorDragged(const ofVec3f &x, const ofVec3f &dx, const int button) { };

	void attachCursor(int button);
	void detachCursor(int button);
	bool cursorAttached(int button);
	
protected:
	bool hasCursorAttached[3];
};