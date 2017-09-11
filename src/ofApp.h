#pragma once

#include "ofMain.h"
#include "FlowManager.h"
#include "ofxXmlSettings.h"
#include "ofxUI.h"
#include "ONILooper.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void updateUser();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void onGui(ofxUIEventArgs & e);
    void syncGui(string fName);
    void onButSaveAs();
    void onButLoad();
	void quickSave();
	void quickLoad();
    
    ofVec2f mapScreenPt2Flow(ofVec2f sPt);
    ofVec2f mapSkelPt2Flow(ofVec2f sPt);
    
    
    ofxUICanvas * gui;
    ofxUILabel * laFile;
    ofxUILabelButton * butClear;
    ofxUILabelButton * butSave;
    ofxUILabelButton * butLoad;
    ofxUILabelToggle * togOniShowSkel;
    ofxUILabelToggle * togOniShowDpt;
    ofxUILabelToggle * togOniUseHl;
    ofxUILabelToggle * togOniUseHr;
    ofxUILabelToggle * togOniUseDpt;
    ofxUILabelToggle * togFluid;
    ofxUILabelToggle * togParts;
    ofxUISlider * sliPartSz;
    ofxUISlider * sliPartSzSpread;
    ofxUISlider * sliPartGx;
    ofxUISlider * sliPartGy;
	ofxUISlider * sliPartR;
	ofxUISlider * sliPartG;
	ofxUISlider * sliPartB;
	ofxUISlider * sliPartA;
    ofxUISlider * sliPartMass;
    ofxUISlider * sliPartMassSpread;
    ofxUISlider * sliPartLifeSpan;
    ofxUISlider * sliPartLifeSpanSpread;
    ofxUISlider * sliPartBirth;
    ofxUISlider * sliPartBirthVel;
    ofxUISlider * sliFluidSpeed;
    ofxUISlider * sliFluidCell;
    ofxUISlider * sliFluidViscosity;
    ofxUISlider * sliFluidVorticity;
    ofxUISlider * sliFluidDissipation;
    ofxUISlider * sliFluidSigma;
    ofxUISlider * sliFluidWeight;
    ofxUISlider * sliFluidGx;
    ofxUISlider * sliFluidGy;
    ofxUISlider * sliFrcR;
    ofxUISlider * sliFrcG;
    ofxUISlider * sliFrcB;
    ofxUISlider * sliFrcA;
    ofxUISlider * sliFrcStrength;
    ofxUISlider * sliFrcRadius;
    ofxUISlider * sliFrcEdge;
    ofxUISlider * sliOfStrength;
    ofxUISlider * sliOfOffset;
    ofxUISlider * sliOfLambda;
    ofxUISlider * sliOfThr;
    ofxUILabelToggle * togOfIx;
    ofxUILabelToggle * togOfIy;
    
    FlowManager fm;
    ONILooper oni;
    ofFbo fboDpt;
    ofImage imgPixels;
	string sKS;
    ofRectangle rOni;
    ofVec2f lastMouse;
    ofVec2f lastHlx;
    ofVec2f lastHrx;
	string lastPth;
    float lastTime;
    float deltaTime;
    bool bShowSkel;
    bool bShowDpt;
    bool bUseLx;
    bool bUseRx;
    bool bUseDpt;
    
    bool bJustLoaded;
    int loadFrame;
};