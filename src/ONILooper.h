#pragma once
#include "ofxNI2.h"
#include "ofxNiTE2.h"
#include "ofxXmlSettings.h"

class ONILooper {
public:
	~ONILooper();
	void setup();
	void update();
	void drawSkeleton(ofRectangle r);
	void checkONIfile();
	bool isONIready();
	void closeAllONI();
    
	void loadXml();
    
	ofxNI2::Device dev;
	ofxNI2::DepthStream dpt;
	ofxNiTE2::UserTracker uTrack;
    
	string pthONI;
	ofVec2f dptRes;
	ofVec2f offset;
	ofVec2f scale;
};