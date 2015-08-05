#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( ){

	ofGLFWWindowSettings windowSettings;
	windowSettings.setGLVersion(3, 1);
	windowSettings.width = 1280;
	windowSettings.height = 1024;
	windowSettings.windowMode = OF_WINDOW;

	ofCreateWindow(windowSettings);

	ofRunApp( new testApp());
}
