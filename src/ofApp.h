
#pragma once

#include "ofMain.h"
#include "ofxFaceTracker2.h"

class ofApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();

	void keyReleased(int key);
	
	
	ofxFaceTracker2 tracker;
	void  loadFaceTracker();

	vector<ofFile> files;
	size_t currentIndex = 0;
	void loadNext();
	void loadDialog();
	
	ofImage img;
	bool bSaveFace = false;
	
	string croppedDirPath;

	bool bDone = true;
	
	bool bCropFirstOnly = false;

	void drawCroppedFace();
	
	void drawHelpString();
	
	size_t currentFaceIndex = 0;
	
};

