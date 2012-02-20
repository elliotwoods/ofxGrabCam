#pragma once
//
//  of3dMouseEventArgs.h
//  ofxGrabCam
//
//  Created by Elliot Woods on 10/11/2011.
//	http://www.kimchiandchips.com
//

#include "ofVec3f.h"
#include "ofVec2f.h"

///Prototype of 3d events class
class of3dMouseEventArgs {
public:
	of3dMouseEventArgs(int button, const ofVec3f& world, const ofVec3f& worldMovement, const ofVec3f& screen, const ofVec3f& screenMovement);
	
	int getButton() const;
	const ofVec3f& getWorld() const;
	const ofVec3f& getWorldMovement() const;
	const ofVec2f& getScreen() const;
	const ofVec2f& getScreenMovement() const;

protected:
	int button;
	ofVec3f world;
	ofVec3f worldMovement;
	ofVec2f screen;
	ofVec2f screenMovement;
};

///Interface for classes that receive mouse events
class of3dMouseEventArgsListener {
public:
	virtual bool isInside(const ofVec3f& position) const = 0;
	virtual void mousePressed(const of3dMouseEventArgs& action) { };
	virtual void mouseDragged(const of3dMouseEventArgs& action) { };
	virtual void mouseReleased(const of3dMouseEventArgs& action) { };
};