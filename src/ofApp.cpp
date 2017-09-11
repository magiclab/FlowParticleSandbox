#include "ofApp.h"

ofVec2f pp;

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(100);
	ofSetWindowTitle("FlowTool");
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
    
    bJustLoaded = false;
    
	bShowSkel = false;
	bShowDpt = false;
	bUseLx = false;
	bUseRx = false;
	bUseDpt = false;
    
	lastPth = "";
    
	oni.setup();
	//rOni.set(0, 0, 640, 480);
	//rOni.x = 1280 / 2 - 320 + 200;
	//rOni.y = 720 / 2 - 240;
	rOni.set(oni.offset.x, oni.offset.y, oni.scale.x*640, oni.scale.y*480);
    
	fboDpt.allocate(1280, 720, GL_RGB);
	fboDpt.begin();
	ofClear(0, 0, 0, 255);
	fboDpt.end();
    
	fm.setup(1280, 720, 1280 / 4, 720 / 4);
    
	lastTime = ofGetElapsedTimef();
	deltaTime = 0;
    
	sKS = "Keyboard shortcuts:\n";
	sKS += "[space] clear screen\n";
	sKS += "[S] save as      [s] quick save\n";
	sKS += "[L] load file      [l] quick load\n";
    
	laFile = new ofxUILabel("-", OFX_UI_FONT_SMALL);
	butClear = new ofxUILabelButton("clear", false);
	butSave = new ofxUILabelButton("Save As",false);
	butLoad = new ofxUILabelButton("Load", false);
	togOniShowSkel = new ofxUILabelToggle("show skel", &bShowSkel, 90, 20);
	togOniShowDpt = new ofxUILabelToggle("show dpt", &bShowDpt, 90, 20);
	togOniUseHl = new ofxUILabelToggle("use hand Lx", &bUseLx, 90, 20);
	togOniUseHr = new ofxUILabelToggle("use hand Rx", &bUseRx, 90, 20);
	togOniUseDpt = new ofxUILabelToggle("use silhouette", &bUseDpt, 180, 20);
	togParts = new ofxUILabelToggle("Particles", fm.particleFlow.isActive());
	sliPartR = new ofxUISlider("R", 0, 1, &fm.particleFlow.getParticleColor().r, 180, 10);
	sliPartG = new ofxUISlider("G", 0, 1, &fm.particleFlow.getParticleColor().g, 180, 10);
	sliPartB = new ofxUISlider("B", 0, 1, &fm.particleFlow.getParticleColor().b, 180, 10);
	sliPartA = new ofxUISlider("A", 0, 1, &fm.particleFlow.getParticleColor().a, 180, 10);
	sliPartSz = new ofxUISlider("size", 1, 10, fm.particleFlow.getSize(), 90, 10);
	sliPartSzSpread = new ofxUISlider("spread",0,10,fm.particleFlow.getSizeSpread(), 90,10);
	sliPartGx = new ofxUISlider("G x",-10,10,fm.particleFlow.getGravity().x,90,10);
	sliPartGy = new ofxUISlider("G y", -10, 10, fm.particleFlow.getGravity().y, 90, 10);
	sliPartMass = new ofxUISlider("mass", 0, 2, fm.particleFlow.getMass(), 90, 10);
	sliPartMassSpread = new ofxUISlider("spread", 0, 1, fm.particleFlow.getMassSpread(), 90, 10);
	sliPartLifeSpan = new ofxUISlider("life",0,10,fm.particleFlow.getLifeSpan(),90,10);
	sliPartLifeSpanSpread = new ofxUISlider("spread", 0, 1, fm.particleFlow.getLifeSpanSpread(), 90, 10);
	sliPartBirth = new ofxUISlider("birth",0,1,fm.particleFlow.getBirthChance(),90,10);
	sliPartBirthVel = new ofxUISlider("vel", 0, 1, fm.particleFlow.getBirthVelocityChance(), 90, 10);
	sliFluidSpeed = new ofxUISlider("speed",0,120,fm.fluid.getSpeed(),90,10);
	sliFluidCell = new ofxUISlider("cell",0,2,fm.fluid.getCellSize(),90,10);
	sliFluidViscosity = new ofxUISlider("viscosity", 0, 0.5, fm.fluid.getViscosity(), 180, 10);
	sliFluidVorticity = new ofxUISlider("vorticity", 0,1,fm.fluid.getVorticity(), 180,10);
	sliFluidDissipation = new ofxUISlider("dissipation",0,0.01,fm.fluid.getDissipation(),180,10);
	sliFluidSigma = new ofxUISlider("sigma",0,1,fm.fluid.getSmokeSigma(),90,10);
	sliFluidWeight = new ofxUISlider("weight",0,1,fm.fluid.getSmokeWeight(),90,10);
	sliFluidGx = new ofxUISlider("G x", -10, 10, fm.fluid.getGravity().x, 90, 10);
	sliFluidGy = new ofxUISlider("G y", -10, 10, fm.fluid.getGravity().y, 90, 10);
	sliFrcR = new ofxUISlider("R", 0, 1, fm.flexDrawForces[0].getForce().x, 180, 10);
	sliFrcG = new ofxUISlider("G", 0, 1, fm.flexDrawForces[0].getForce().y, 180, 10);
	sliFrcB = new ofxUISlider("B", 0, 1, fm.flexDrawForces[0].getForce().z, 180, 10);
	sliFrcA = new ofxUISlider("A", 0, 1, fm.flexDrawForces[0].getForce().w, 180, 10);
	sliFrcStrength = new ofxUISlider("strength",0,5,fm.flexDrawForces[0].getStrength(), 180,10);
	sliFrcRadius = new ofxUISlider("radius", 0, 0.1, fm.flexDrawForces[0].getRadius(), 90, 10);
	sliFrcEdge = new ofxUISlider("edge",0,1,fm.flexDrawForces[0].getEdge(),90,10);
	sliOfStrength = new ofxUISlider("strength",0,100,fm.opticalFlow.getStrength(),90,10);
	sliOfOffset = new ofxUISlider("offset",1,10,fm.opticalFlow.getOffset(),90,10);
	sliOfLambda = new ofxUISlider("lambda",0.1,1,fm.opticalFlow.getLambda(),90,10);
	sliOfThr = new ofxUISlider("thresh", 0, 0.2, fm.opticalFlow.getThreshold(), 90, 10);
	togOfIx = new ofxUILabelToggle("mirror X", fm.opticalFlow.getInverseX(), 90, 10);
	togOfIy = new ofxUILabelToggle("mirror Y", fm.opticalFlow.getInverseY(), 90, 10);
    togFluid = new ofxUILabelToggle("Fluid",&fm.bFluid);
    
	gui = new ofxUICanvas(0,0,200,ofGetHeight());
	gui->addWidgetDown(new ofxUILabel("FlowTool",OFX_UI_FONT_LARGE));
	gui->addWidgetDown(butSave);
	gui->addWidgetRight(butLoad);
	gui->addWidgetRight(butClear);
	gui->addWidgetDown(new ofxUILabel("preset name:", OFX_UI_FONT_SMALL));
	gui->addWidgetDown(laFile);
	gui->addSpacer();
	gui->addWidgetDown(new ofxUILabel("ONI", OFX_UI_FONT_LARGE));
	gui->addWidgetDown(togOniShowSkel);
	gui->addWidgetRight(togOniShowDpt);
	gui->addWidgetDown(togOniUseHl);
	gui->addWidgetRight(togOniUseHr);
	gui->addWidgetDown(togOniUseDpt);
	gui->addSpacer();
	gui->addWidgetDown(togParts);
	gui->addWidgetDown(sliPartR);
	gui->addWidgetDown(sliPartG);
	gui->addWidgetDown(sliPartB);
	gui->addWidgetDown(sliPartA);
	gui->addWidgetDown(sliPartSz);
	gui->addWidgetRight(sliPartSzSpread);
	gui->addWidgetDown(sliPartGx);
	gui->addWidgetRight(sliPartGy);
	gui->addWidgetDown(sliPartMass);
	gui->addWidgetRight(sliPartMassSpread);
	gui->addWidgetDown(sliPartLifeSpan);
	gui->addWidgetRight(sliPartLifeSpanSpread);
	gui->addWidgetDown(sliPartBirth);
	gui->addWidgetRight(sliPartBirthVel);
	gui->addSpacer();
    gui->addWidgetDown(togFluid);
	gui->addWidgetDown(sliFluidSpeed);
	gui->addWidgetRight(sliFluidCell);
	gui->addWidgetDown(sliFluidViscosity);
	gui->addWidgetDown(sliFluidVorticity);
	gui->addWidgetDown(sliFluidDissipation);
	gui->addWidgetDown(sliFluidSigma);
	gui->addWidgetRight(sliFluidWeight);
	gui->addWidgetDown(sliFluidGx);
	gui->addWidgetRight(sliFluidGy);
	gui->addSpacer();
	gui->addWidgetDown(new ofxUILabel("Force", OFX_UI_FONT_MEDIUM));
	gui->addWidgetDown(sliFrcR);
	gui->addWidgetDown(sliFrcG);
	gui->addWidgetDown(sliFrcB);
	gui->addWidgetDown(sliFrcA);
	gui->addWidgetDown(sliFrcStrength);
	gui->addWidgetDown(sliFrcRadius);
	gui->addWidgetRight(sliFrcEdge);
	gui->addSpacer();
	gui->addWidgetDown(new ofxUILabel("Optical Flow", OFX_UI_FONT_MEDIUM));
	gui->addWidgetDown(sliOfStrength);
	gui->addWidgetRight(sliOfOffset);
	gui->addWidgetDown(sliOfLambda);
	gui->addWidgetRight(sliOfThr);
	gui->addWidgetDown(togOfIx);
	gui->addWidgetRight(togOfIy);
    
	ofAddListener(gui->newGUIEvent, this, &ofApp::onGui);
}

void ofApp::syncGui(string fName) {
	laFile->setLabel(fName);
	togParts->setValue(fm.particleFlow.isActive());
	sliPartSz->setValue(fm.particleFlow.getSize());
	sliPartSzSpread->setValue(fm.particleFlow.getSizeSpread());
	sliPartGx->setValue(fm.particleFlow.getGravity().x);
	sliPartGy->setValue(fm.particleFlow.getGravity().y);
	sliPartMass->setValue(fm.particleFlow.getMass());
	sliPartMassSpread->setValue(fm.particleFlow.getMassSpread());
	sliPartLifeSpan->setValue(fm.particleFlow.getLifeSpanSpread());
	sliPartBirth->setValue(fm.particleFlow.getBirthChance());
	sliPartBirthVel->setValue(fm.particleFlow.getBirthVelocityChance());
	sliFluidSpeed->setValue(fm.fluid.getSpeed());
	sliFluidCell->setValue(fm.fluid.getCellSize());
	sliFluidViscosity->setValue(fm.fluid.getViscosity());
	sliFluidVorticity->setValue(fm.fluid.getVorticity());
	sliFluidDissipation->setValue(fm.fluid.getDissipation());
	sliFluidSigma->setValue(fm.fluid.getSmokeSigma());
	sliFluidWeight->setValue(fm.fluid.getSmokeWeight());
	sliFluidGx->setValue(fm.fluid.getGravity().x);
	sliFluidGy->setValue(fm.fluid.getGravity().y);
	sliFrcR->setValue(fm.flexDrawForces[0].getForce().x);
	sliFrcG->setValue(fm.flexDrawForces[0].getForce().y);
	sliFrcB->setValue(fm.flexDrawForces[0].getForce().z);
	sliFrcA->setValue(fm.flexDrawForces[0].getForce().w);
	sliFrcStrength->setValue(fm.flexDrawForces[0].getStrength());
	sliFrcRadius->setValue(fm.flexDrawForces[0].getRadius());
	sliFrcEdge->setValue(fm.flexDrawForces[0].getEdge());
	sliOfStrength->setValue(fm.opticalFlow.getStrength());
	sliOfOffset->setValue(fm.opticalFlow.getOffset());
	sliOfLambda->setValue(fm.opticalFlow.getLambda());
	sliOfThr->setValue(fm.opticalFlow.getThreshold());
	togOfIx->setValue(fm.opticalFlow.getInverseX());
	togOfIy->setValue(fm.opticalFlow.getInverseY());
}

void ofApp::onButSaveAs() {
	ofFileDialogResult dr = ofSystemSaveDialog( "default.flow", "save as...");
	if (dr.bSuccess) {
		fm.save(dr.getPath());
		laFile->setLabel(dr.getName());
		lastPth = dr.getPath();
	}
	else {
		cout << "ERROR: cannot save file" << endl;
	}
}

void ofApp::quickSave() {
	if (lastPth != "") {
		fm.save(lastPth);
	}
	else {
		ofSystemAlertDialog("No path specified: Save As/ Load As first");
	}
}

void ofApp::onButLoad() {
	ofFileDialogResult dr = ofSystemLoadDialog("Load", false, ofToDataPath("./"));
	if (dr.bSuccess) {
		fm.load(dr.getPath());
		syncGui(dr.getName());
		lastPth = dr.getPath();
        bJustLoaded = true;
        loadFrame = ofGetFrameNum();
	}
	else {
		cout << "ERROR: cannot load file" << endl;
	}
}

void ofApp::quickLoad() {
	if (lastPth != "") {
		fm.load(lastPth);
		syncGui(ofFilePath::getFileName(lastPth));
	}
	else {
		ofSystemAlertDialog("No path specified: Save As/ Load As first");
	}
}

void ofApp::onGui(ofxUIEventArgs & e) {
	if (e.widget == togParts) {
		fm.particleFlow.activate(togParts->getValue());
	}
	else if (e.widget == togOniUseHl) {
		
	}
	else if (e.widget == togOniUseHr) {
        
	}
	else if (e.widget == togOfIx) {
		fm.opticalFlow.setInverseX(togOfIx->getValue());
	}
	else if (e.widget == togOfIy) {
		fm.opticalFlow.setInverseY(togOfIy->getValue());
	}
	else if(e.widget == sliPartSz) {
		fm.particleFlow.setSize(sliPartSz->getValue());
	}
	else if (e.widget == sliPartSzSpread) {
		fm.particleFlow.setSizeSpread(sliPartSzSpread->getValue());
	}
	else if (e.widget == sliPartMass) {
		fm.particleFlow.setMass(sliPartMass->getValue());
	}
	else if (e.widget == sliPartMassSpread) {
		fm.particleFlow.setMassSpread(sliPartMassSpread->getValue());
	}
	else if (e.widget == sliPartLifeSpan) {
		fm.particleFlow.setLifeSpan(sliPartLifeSpan->getValue());
	}
	else if (e.widget == sliPartLifeSpanSpread) {
		fm.particleFlow.setLifeSpanSpread(sliPartLifeSpanSpread->getValue());
	}
	else if (e.widget == sliPartBirth) {
		fm.particleFlow.setBirthChance(sliPartBirth->getValue());
	}
	else if (e.widget == sliPartBirthVel) {
		fm.particleFlow.setBirthVelocityChance(sliPartBirthVel->getValue());
	}
	else if (e.widget == sliPartGx || e.widget== sliPartGy) {
		fm.particleFlow.setGravity(ofVec2f(sliPartGx->getValue(), sliPartGy->getValue()));
	}
	else if (e.widget == sliFluidSpeed) {
		fm.fluid.setSpeed(sliFluidSpeed->getValue());
	}
	else if (e.widget == sliFluidCell) {
		fm.fluid.setCellSize(sliFluidCell->getValue());
	}
	else if (e.widget == sliFluidViscosity) {
		fm.fluid.setViscosity(sliFluidViscosity->getValue());
	}
	else if (e.widget == sliFluidVorticity) {
		fm.fluid.setVorticity(sliFluidVorticity->getValue());
	}
	else if (e.widget == sliFluidDissipation) {
		fm.fluid.setDissipation(sliFluidDissipation->getValue());
	}
	else if (e.widget == sliFluidSigma) {
		fm.fluid.setSmokeSigma(sliFluidSigma->getValue());
	}
	else if (e.widget == sliFluidWeight) {
		fm.fluid.setSmokeWeight(sliFluidWeight->getValue());
	}
	else if (e.widget == sliFluidGx || e.widget==sliFluidGy) {
		fm.fluid.setGravity(ofVec2f(sliFluidGx->getValue(), sliFluidGy->getValue()));
	}
	else if (e.widget == sliFrcR || e.widget == sliFrcG || e.widget == sliFrcB || e.widget == sliFrcA) {
		fm.flexDrawForces[0].setForce(ofVec4f(sliFrcR->getValue(), sliFrcG->getValue(), sliFrcB->getValue(), sliFrcA->getValue()));
	}
	else if (e.widget == sliFrcStrength) {
		fm.flexDrawForces[0].setStrength(sliFrcStrength->getValue());
	}
	else if (e.widget == sliFrcRadius) {
		fm.flexDrawForces[0].setRadius(sliFrcRadius->getValue());
	}
	else if (e.widget == sliFrcEdge) {
		fm.flexDrawForces[0].setEdge(sliFrcEdge->getValue());
	}
	else if (e.widget == sliOfStrength) {
		fm.opticalFlow.setStrength(sliOfStrength->getValue());
	}
	else if (e.widget == sliOfOffset) {
		fm.opticalFlow.setOffset(sliOfOffset->getValue());
	}
	else if (e.widget == sliOfLambda) {
		fm.opticalFlow.setLambda(sliOfLambda->getValue());
	}
	else if (e.widget == sliOfThr) {
		fm.opticalFlow.setThreshold(sliOfThr->getValue());
	}
	else if (e.widget == butSave) {
		if (butSave->getValue()) {
			onButSaveAs();
		}
	}
	else if (e.widget == butLoad) {
		if (butLoad->getValue()) {
			onButLoad();
		}
	}
	else if (e.widget == butClear) {
		if (butClear->getValue()) {
			fm.fluid.reset();
		}
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	deltaTime = ofGetElapsedTimef() - lastTime;
	lastTime = ofGetElapsedTimef();
	
	oni.update();
	
	updateUser();
    
	fm.update(deltaTime);
    
}

void ofApp::updateUser() {
	if (oni.uTrack.getNumUser() > 0) {
		for (int i = 0; i < oni.uTrack.getNumUser(); i++) {
			ofxNiTE2::User::Ref ur = oni.uTrack.getUser(i);
			if (ur->isVisible()) {
				nite::Point3f pL = oni.uTrack.getWorldPosForJoint(ur, nite::JOINT_LEFT_HAND);
				nite::Point3f pR = oni.uTrack.getWorldPosForJoint(ur, nite::JOINT_RIGHT_HAND);
				ofVec3f wL = ofVec3f(pL.x, pL.y, pL.z);
				ofVec3f wR = ofVec3f(pR.x, pR.y, pR.z);
				ofVec2f sL = oni.uTrack.getScreenPosForPoint(wL, ofRectangle(0,0,rOni.width,rOni.height));
				ofVec2f sR = oni.uTrack.getScreenPosForPoint(wR, ofRectangle(0, 0, rOni.width, rOni.height));
                
				if (bUseDpt) {
					nite::Point3f p = oni.uTrack.getWorldPosForJoint(ur, nite::JOINT_TORSO);
					ofVec3f v(p.x,p.y,p.z);
					float dMin = v.z - 250;// - 200;
					float dMax = v.z + 250;// + 200;
					imgPixels.setFromPixels(oni.dpt.getThresoldedPixels(dMin, dMax));
                    
					ofPushStyle();
					ofEnableBlendMode(OF_BLENDMODE_DISABLED);
					fboDpt.begin();
					ofClear(0, 0, 0, 255);
					ofFloatColor cc;
					cc.set(fm.flexDrawForces[0].getForce().x, fm.flexDrawForces[0].getForce().y, fm.flexDrawForces[0].getForce().z, fm.flexDrawForces[0].getForce().w);
					ofSetColor(cc);
					imgPixels.draw(rOni.x-200, rOni.y, rOni.width, rOni.height);
					fboDpt.end();
					ofPopStyle();
                    
					fm.applyOpticalFlow(fboDpt.getTexture());
				}
                
				if (bUseLx) {
					ofVec2f lh = mapSkelPt2Flow(sL);
					fm.applyForce(lh, lastHlx);
				}
                
				if (bUseRx) {
					ofVec2f rh = mapSkelPt2Flow(sR);
					fm.applyForce(rh, lastHrx);
				}
                
				i = oni.uTrack.getNumUser() + 2;
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofPushMatrix();
	ofTranslate(200, 0);
	ofFill();
	ofSetColor(ofColor::black);
	ofDrawRectangle(0, 0, fm.drawWidth, fm.drawHeight);
	ofSetColor(ofColor::white);
	ofPushStyle();
	if(fm.bFluid)fm.fluid.draw(0, 0, fm.drawWidth, fm.drawHeight,1);
	if (fm.particleFlow.isActive()) {
		fm.particleFlow.draw(0, 0, fm.drawWidth, fm.drawHeight,1);
	}
	if (bShowDpt) {
		ofSetColor(255, 255, 255, 80);
		oni.dpt.draw(rOni.x - 200, rOni.y, rOni.width, rOni.height);
	}
	ofPopStyle();
    
	
	if(bShowSkel)oni.drawSkeleton(rOni);
	ofPopMatrix();
	
    
	ofPushStyle();
	ofSetColor(ofColor::white);
	gui->getFontMedium()->drawString(sKS, 220, fm.drawHeight + 20);
	ofPopStyle();
	
	//fboDpt.draw(200, 0);
	//ofPushStyle();
	//ofSetColor(ofColor::red);
	//ofDrawCircle(pp.x, pp.y, 20, 20);
	//ofPopStyle();
    
    if(bJustLoaded){
        if(ofGetFrameNum()>(loadFrame+3)){
            fm.fluid.reset();
            fm.particleFlow.clear();
            bJustLoaded = false;
            cout<<"BOOM"<<endl;
        }else{
            
        }
    }
}

ofVec2f ofApp::mapSkelPt2Flow(ofVec2f sPt) {
	ofVec2f fPt = sPt;
	
	fPt.x = rOni.width - fPt.x + rOni.x;
	fPt.y = rOni.height - fPt.y + rOni.y;
    
	fPt = mapScreenPt2Flow(fPt);
	return fPt;
}

ofVec2f ofApp::mapScreenPt2Flow(ofVec2f sPt) {
	ofVec2f fPt;
	fPt.x = ofMap(sPt.x, 200, 200.0+fm.drawWidth,0.0,1.0);
	fPt.y = sPt.y / float(fm.drawHeight);
	return fPt;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key)
	{
        case ' ':
            fm.fluid.reset();
            fm.particleFlow.clear();
            break;
        case 'S':
            onButSaveAs();
            break;
        case 's':
            quickSave();
            break;
        case 'L':
            onButLoad();
            break;
        case 'l':
            quickLoad();
            break;
        default:
            break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	lastMouse = mapScreenPt2Flow(ofVec2f(x,y));
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	if (x < 200)return;
	ofVec2f mm = mapScreenPt2Flow(ofVec2f(x,y));
    
	fm.applyForce(mm, lastMouse);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}