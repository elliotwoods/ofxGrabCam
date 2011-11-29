//
//  ofxInteractiveNode.cpp
//  ofxGrabCam
//
//  Created by Elliot Woods on 10/11/2011.
//  Copyright (c) 2011 Kimchi and Chips. All rights reserved.
//

#include "ofxInteractiveNode.h"

void ofxInteractiveNode::attachCursor(int button) {
	if (button >= 0 && button <3)
		hasCursorAttached[button] = true;
}

void ofxInteractiveNode::detachCursor(int button) {
	if (button >= 0 && button <3)
		hasCursorAttached[button] = false;
}

bool ofxInteractiveNode::cursorAttached(int button) {
	if (button >= 0 && button <3)
		return hasCursorAttached[button];
	else
		return false;
}

