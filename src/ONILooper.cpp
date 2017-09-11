#include "ONILooper.h"

ONILooper::~ONILooper() {
	closeAllONI();
}

void ONILooper::loadXml() {
	offset.set(0, 0);
	scale.set(1, 1);
	ofxXmlSettings xml;
	if(xml.load("xml/skeleton.xml")) {
		if (xml.tagExists("skeleton")) {
			xml.pushTag("skeleton");
			if (xml.tagExists("offset")) {
				xml.pushTag("offset");
				offset.x = xml.getValue("x",offset.x);
				offset.y = xml.getValue("y", offset.y);
				xml.popTag();
			}
			if (xml.tagExists("scale")) {
				xml.pushTag("scale");
				scale.x = xml.getValue("x", scale.x);
				scale.y = xml.getValue("y", scale.y);
				xml.popTag();
			}
			xml.popTag();
		}
		else {
			ofLogError("ONILooper::loadXml", "malformed xml file");
		}
	}
	else {
		ofLogError("ONILooper::loadXml","cannot load xml file");
	}
    
	cout << "ONILooper: offset: " << offset << "; scale: " << scale << endl;
}

void ONILooper::checkONIfile() {
	ofDirectory dir(ofToDataPath("oni"));
	dir.allowExt("oni");
	vector<ofFile> ff = dir.getFiles();
	if (ff.size() > 0) {
		pthONI = ff[0].getAbsolutePath();
		cout << "ONILooper: loading " << pthONI << endl;
	}
	else {
		ofLogError("ONILooper::checkONIfile","no oni files available");
	}
}

bool ONILooper::isONIready() {
	bool b = false;
	if (pthONI.size() >= 4) {
		string ext = pthONI.substr(pthONI.size() - 4, 4);
		b = (ext == ".oni");
	}
	return b;
}

void ONILooper::setup() {
	checkONIfile();
	if (isONIready()) {
		dev.setup(pthONI);
	}
	else {
		ofLogError("ONILooper::setup","cannot load "+pthONI);
	}
    
	loadXml();
    
	uTrack.setup(dev);
	dptRes.set(uTrack.getPixelsRef().getWidth(), uTrack.getPixelsRef().getHeight());
	dpt.setup(dev);
	dpt.setSize(dptRes.x, dptRes.y);
	dpt.setFps(30);
	dpt.start();
}

void ONILooper::closeAllONI() {
	uTrack.exit();
	uTrack.get().destroy();
    
	dpt.get().destroy();
	dpt.exit();
    
	dev.get().close();
	dev.exit();
}

void ONILooper::update() {
	dev.update();
}

void ONILooper::drawSkeleton(ofRectangle r) {
	ofPushView();
	ofPushStyle();
	ofSetColor(255, 255, 255, 200);
	ofSetLineWidth(2);
	uTrack.getOverlayCamera().begin(r);
	if (uTrack.getNumUser() > 0) {
		uTrack.draw();
	}
	uTrack.getOverlayCamera().end();
	ofPopStyle();
	ofPopView();
}