#pragma once
#include "ofMain.h"
#include "ofxFlowTools.h"
#include "ofxXmlSettings.h"

class FlowManager {
public:
	FlowManager();
	~FlowManager();
    
	void setup(int drawW, int drawH, int flowW, int flowH);
	void update(float & deltaTime);
    
	void applyForce(ofVec2f & curPt, ofVec2f & lastPt);
	void applyOpticalFlow(ofTexture & txt);
    
	void setDrawResolution(int ww, int hh);
	void setFlowResolution(int ww, int hh);
    
	void save(string pth);
	bool load(string pth);
    
	int flowWidth;
	int flowHeight;
	int drawWidth;
	int drawHeight;
    
	flowTools::ftOpticalFlow opticalFlow;
	flowTools::ftVelocityMask velocityMask;
	flowTools::ftFluidSimulation fluid;
	flowTools::ftParticleFlow particleFlow;
    
	flowTools::ftDisplayScalar displayScalar;
	flowTools::ftVelocityField velocityField;
	flowTools::ftTemperatureField temperatureField;
    
	int numDrawForces;
	flowTools::ftDrawForce * flexDrawForces;
    bool bFluid;
    
};