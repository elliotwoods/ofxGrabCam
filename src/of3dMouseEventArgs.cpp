//
//  of3dMouseEventArgs.cpp
//  ofxGrabCam
//
//  Created by Elliot Woods on 10/11/2011.
//	http://www.kimchiandchips.com
//
#include "of3dMouseEventArgs.h"

of3dMouseEventArgs::of3dMouseEventArgs(int button, const ofVec3f& world, const ofVec3f& worldMovement, const ofVec3f& screen, const ofVec3f& screenMovement) {
	this->button = button;
	this->world = world;
	this->worldMovement = worldMovement;
	this->screen = screen;
	this->screenMovement = screenMovement;
}

int of3dMouseEventArgs::getButton() const {
	return this->button;
}

const ofVec3f& of3dMouseEventArgs::getWorld() const {
	return this->world;
}

const ofVec3f& of3dMouseEventArgs::getWorldMovement() const {
	return this->worldMovement;
}

const ofVec2f& of3dMouseEventArgs::getScreen() const {
	return this->screen;
}

const ofVec2f& of3dMouseEventArgs::getScreenMovement() const {
	return this->screenMovement;
}