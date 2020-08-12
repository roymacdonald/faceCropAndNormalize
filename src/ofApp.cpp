#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	

	loadFaceTracker();
	
	
	loadDialog();
	
	ofSetWindowShape(400, 400);
	loadNext();
	
}

//--------------------------------------------------------------
void ofApp::loadFaceTracker()
{
	// Setup tracker
	tracker.setThreaded(false);
	
	
	
	ofFile dat(ofToDataPath("model/shape_predictor_68_face_landmarks.dat"));

	cout << dat.getAbsolutePath() << "\n";
	
	if(!dat.exists())
	{
		ofFile addonDat(ofFile(__BASE_FILE__).getEnclosingDirectory()+"../../../../addons/ofxFaceTracker2/model/shape_predictor_68_face_landmarks.dat");
		if(addonDat.exists())
		{
			addonDat.copyTo(dat.getAbsolutePath());
		}
		else
		{
			ofLogError("ofApp::loadFaceTracker") << "face tracker model missing!";
			ofExit();
		}
	}
	
	tracker.setup(dat.getAbsolutePath());

}

//--------------------------------------------------------------
void ofApp::loadDialog(){
	auto d = ofSystemLoadDialog("Select a folder with the images (png or jpeg)", true);
	if (d.bSuccess) {
		ofDirectory dir(d.getPath());
		ofDirectory croppedDir(ofFilePath::join(d.getPath(), "/cropped"));
		
		if(!croppedDir.exists()){
			croppedDir.create();
		}
		croppedDirPath = croppedDir.getAbsolutePath();
		dir.allowExt("jpg");
		dir.allowExt("png");
		dir.listDir();
		files = dir.getFiles();
		bDone = false;
	}
	
}



//--------------------------------------------------------------
void ofApp::loadNext(){
	if(currentIndex >= files.size()){
		//		ofExit();
		bDone = true;
	}else
		if(img.load(files[currentIndex].getAbsolutePath())){
			cout << "Loaded: " << files[currentIndex].getAbsolutePath() << "\n";
			if(tracker.update(img)){
				cout << currentIndex << " tracker updated"<< endl;
				currentFaceIndex = 0;
				bSaveFace = true;
			}
			currentIndex++;
			if(tracker.size() == 0){
				loadNext();
			}
		}
		else
		{
			currentIndex++;
			loadNext();
		}
}
//--------------------------------------------------------------
void ofApp::update(){
	
	if(bSaveFace){
	if(bCropFirstOnly || currentFaceIndex == tracker.size())
	{
		bSaveFace =false;
		loadNext();
	}
	}
}
//--------------------------------------------------------------
void ofApp::drawCroppedFace()
{
	
	auto& inst = tracker.getInstances();
	glm::vec3 cE;
	if( currentFaceIndex < inst.size()){
		
		auto lE = inst[currentFaceIndex].getLandmarks().getImageFeature(ofxFaceTracker2Landmarks::LEFT_EYE).getCentroid2D();
		auto rE = inst[currentFaceIndex].getLandmarks().getImageFeature(ofxFaceTracker2Landmarks::RIGHT_EYE).getCentroid2D();
		cE = lE + ((rE - lE)*0.5);
		ofSetColor(255);
		
		ofPushMatrix();
		auto d = glm::normalize(rE - lE);
		
		float angle = atan2(d.y,d.x);
		glm::vec2 drawPos(ofGetWidth()/2, ofGetHeight()/2);
		
		ofTranslate(drawPos);
		ofRotateRad(-angle);
		ofScale(100./glm::length(rE - lE));
		
		img.draw(-cE.x, -cE.y);
		ofPopMatrix();
		
		
		if(bSaveFace){
			auto p = ofFilePath::join(croppedDirPath, ofToString(currentIndex)+"_"+ofToString(currentFaceIndex)+".png");
			cout << "Save to : " << p << "\n";
			ofSaveScreen(p);
			
			++currentFaceIndex;
			
		}
	}
}
//--------------------------------------------------------------
void ofApp::draw(){
	if(!bDone)
	{
		
		drawCroppedFace();
	}
	drawHelpString();
}
//--------------------------------------------------------------
void ofApp::drawHelpString()
{
	stringstream ss;
	
	ss << "Press [key] for: \n";
	ss << "    [o] : load another folder with images\n";
	ss << "    [f] : toggle crop first detected face only\n";
	ss << "\nStatus:\n";
	ss << "    Crop first only: " << boolalpha << bCropFirstOnly << "\n";
	ss << "    Current file " << currentIndex << "/" << files.size() << "\n";
	if(!bCropFirstOnly)
	{
		ss << "    Current Face: " << currentFaceIndex << "/" << tracker.size();
	}
	ofDrawBitmapStringHighlight(ss.str(), 30,30);
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
	if(key == 'o')
	{
		loadDialog();
	}
	else if(key == 'f')
	{
		bCropFirstOnly ^= true;
	}
}
