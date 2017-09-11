#include "FlowManager.h"

FlowManager::FlowManager() {
	flexDrawForces = NULL;
    bFluid = true;
}

FlowManager::~FlowManager() {
	if (flexDrawForces != NULL) {
		delete[] flexDrawForces;
	}
}

void FlowManager::setDrawResolution(int ww, int hh) {
	drawWidth = ww;
	drawHeight = hh;
}

void FlowManager::setFlowResolution(int ww, int hh) {
	flowWidth = ww;
	flowHeight = hh;
}

void FlowManager::setup(int drawW, int drawH, int flowW, int flowH) {
	setDrawResolution(drawW, drawH);
	setFlowResolution(flowW, flowH);
    
	opticalFlow.setup(flowWidth, flowHeight);
	velocityMask.setup(drawWidth, drawHeight);
    
	fluid.setup(flowWidth, flowHeight, drawWidth, drawHeight);
    
	particleFlow.setup(flowWidth, flowHeight, drawWidth, drawHeight);
	particleFlow.setSize(3); //
	particleFlow.setLifeSpan(3);
	particleFlow.setSpeed(5);
    
	//viz
	displayScalar.setup(flowWidth, flowHeight);
	velocityField.setup(flowWidth / 4, flowHeight / 4);
	temperatureField.setup(flowWidth / 4, flowHeight / 4);
    
	//draw forces
	numDrawForces = 6;
	flexDrawForces = new flowTools::ftDrawForce[numDrawForces];
	flexDrawForces[0].setup(drawWidth, drawHeight, flowTools::FT_DENSITY, true);
	flexDrawForces[0].setName("draw full res");
	flexDrawForces[0].setForce(ofVec4f(0.266887, 0.596257, 1, 0.575007));
	flexDrawForces[0].setStrength(2.42295);
	flexDrawForces[0].setRadius(0.0140983);
	flexDrawForces[0].setEdge(1);
	flexDrawForces[1].setup(flowWidth, flowHeight, flowTools::FT_VELOCITY, true);
	flexDrawForces[1].setName("draw flow res 1");
	flexDrawForces[1].setForce(ofVec4f(-0.00520831, 0.00833333, 0, 1));
	flexDrawForces[1].setStrength(0.965223);
	flexDrawForces[1].setRadius(0.0146189);
	flexDrawForces[1].setEdge(1);
	flexDrawForces[2].setup(flowWidth, flowHeight, flowTools::FT_TEMPERATURE, true);
	flexDrawForces[2].setName("draw flow res 2");
	flexDrawForces[2].setForce(ofVec4f(1, 0, 0, 1));
	flexDrawForces[2].setStrength(1);
	flexDrawForces[2].setRadius(0.0245106);
	flexDrawForces[2].setEdge(1);
	flexDrawForces[3].setup(drawWidth, drawHeight, flowTools::FT_DENSITY, false);
	flexDrawForces[3].setName("draw full res");
	flexDrawForces[3].setForce(ofVec4f(1, 0.490008, 0.341261, 0.723755));
	flexDrawForces[3].setStrength(1);
	flexDrawForces[3].setRadius(0.035);
	flexDrawForces[3].setEdge(1);
	flexDrawForces[4].setup(flowWidth, flowHeight, flowTools::FT_VELOCITY, false);
	flexDrawForces[4].setName("draw flow res 1");
	flexDrawForces[4].setForce(ofVec4f(0, -0.00185183, 0, 1));
	flexDrawForces[4].setStrength(1);
	flexDrawForces[4].setRadius(0.0177426);
	flexDrawForces[4].setEdge(1);
	flexDrawForces[5].setup(flowWidth, flowHeight, flowTools::FT_TEMPERATURE, false);
	flexDrawForces[5].setName("draw flow res 2");
	flexDrawForces[5].setForce(ofVec4f(1, 0, 0, 1));
	flexDrawForces[5].setStrength(1);
	flexDrawForces[5].setRadius(0.0281549);
	flexDrawForces[5].setEdge(1);
}

void FlowManager::update(float & deltaTime) {
	for (int i = 0; i < numDrawForces; i++) {
		flexDrawForces[i].update();
		if (flexDrawForces[i].didChange()) {
			float strength = flexDrawForces[i].getStrength();
			if (!flexDrawForces[i].getIsTemporary()) {
				strength *= deltaTime;
			}
            
			switch (flexDrawForces[i].getType())
			{
                case flowTools::FT_DENSITY:
                    fluid.addDensity(flexDrawForces[i].getTexture(), strength);
                    break;
                case flowTools::FT_VELOCITY:
                    fluid.addVelocity(flexDrawForces[i].getTexture(), strength);
                    particleFlow.addFlowVelocity(flexDrawForces[i].getTexture(), strength);
                    break;
                case flowTools::FT_TEMPERATURE:
                    fluid.addTemperature(flexDrawForces[i].getTexture(), strength);
                    break;
                case flowTools::FT_PRESSURE:
                    fluid.addTemperature(flexDrawForces[i].getTexture(), strength);
                    break;
                case flowTools::FT_OBSTACLE:
                    fluid.addTempObstacle(flexDrawForces[i].getTexture());
                    break;
                default:
                    break;
			}
		}
	}
    
	fluid.update();
	if (particleFlow.isActive()) {
		particleFlow.setSpeed(fluid.getSpeed());
		particleFlow.setCellSize(fluid.getCellSize());
		particleFlow.addFluidVelocity(fluid.getVelocity());
		particleFlow.setObstacle(fluid.getObstacle());
	}
	particleFlow.update();
}

void FlowManager::applyForce(ofVec2f & curPt, ofVec2f & lastPt) {
	ofVec2f velocity = curPt - lastPt;
    
	for (int i = 0; i < 3; i++) {
		if (flexDrawForces[i].getType() == flowTools::FT_VELOCITY) {
			flexDrawForces[i].setForce(velocity);
		}
		flexDrawForces[i].applyForce(curPt);
	}
    
	lastPt = curPt;
}

void FlowManager::applyOpticalFlow(ofTexture & txt) {
	opticalFlow.setSource(txt);
	opticalFlow.update();
    
	velocityMask.setDensity(txt);
	velocityMask.setVelocity(opticalFlow.getOpticalFlow());
	velocityMask.update();
    
	fluid.addVelocity(opticalFlow.getOpticalFlowDecay());
    particleFlow.addFlowVelocity(opticalFlow.getOpticalFlowDecay());
	fluid.addDensity(velocityMask.getColorMask());
	fluid.addTemperature(velocityMask.getLuminanceMask());
}

void FlowManager::save(string pth) {
	ofxXmlSettings xml;
	xml.addTag("flow");
	xml.pushTag("flow");
	
	xml.addTag("particles");
	xml.pushTag("particles");
	xml.addValue("active", particleFlow.isActive() ? "1" : "0");
	xml.addValue("size", particleFlow.getSize());
	xml.addValue("sizeSpread",particleFlow.getSizeSpread());
	xml.addValue("mass", particleFlow.getMass());
	xml.addValue("massSpread", particleFlow.getMassSpread());
	xml.addValue("lifeSpan", particleFlow.getLifeSpan());
	xml.addValue("lifeSpanSpread", particleFlow.getLifeSpanSpread());
	xml.addValue("birthChance", particleFlow.getBirthChance());
	xml.addValue("birthVelocityChance", particleFlow.getBirthVelocityChance());
	xml.addTag("gravity");
	xml.addAttribute("gravity", "x", particleFlow.getGravity().x,0);
	xml.addAttribute("gravity", "y", particleFlow.getGravity().y,0);
	xml.addTag("color");
	xml.addAttribute("color", "r", particleFlow.getParticleColor().r, 0);
	xml.addAttribute("color", "g", particleFlow.getParticleColor().g, 0);
	xml.addAttribute("color", "b", particleFlow.getParticleColor().b, 0);
	xml.addAttribute("color", "a", particleFlow.getParticleColor().a, 0);
	xml.popTag();
	
	xml.addTag("fluid");
	xml.pushTag("fluid");
    xml.addValue("active",bFluid?"1":"0");
	xml.addValue("speed", fluid.getSpeed());
	xml.addValue("cellSize", fluid.getCellSize());
	xml.addValue("viscosity", fluid.getViscosity());
	xml.addValue("vorticity", fluid.getVorticity());
	xml.addValue("dissipation", fluid.getDissipation());
	xml.addTag("smoke");
	xml.addAttribute("smoke", "sigma", fluid.getSmokeSigma(), 0);
	xml.addAttribute("smoke", "weight", fluid.getSmokeWeight(), 0);
	xml.addTag("gravity");
	xml.addAttribute("gravity", "x", fluid.getGravity().x, 0);
	xml.addAttribute("gravity", "y", fluid.getGravity().y, 0);
	xml.popTag();
    
	xml.addTag("force");
	xml.pushTag("force");
	xml.addTag("color");
	xml.addAttribute("color", "r", flexDrawForces[0].getForce().x, 0);
	xml.addAttribute("color", "g", flexDrawForces[0].getForce().y, 0);
	xml.addAttribute("color", "b", flexDrawForces[0].getForce().z, 0);
	xml.addAttribute("color", "a", flexDrawForces[0].getForce().w, 0);
	xml.addValue("strength", flexDrawForces[0].getStrength());
	xml.addValue("radius", flexDrawForces[0].getRadius());
	xml.addValue("edge", flexDrawForces[0].getEdge());
	xml.popTag();
    
	xml.addTag("of");
	xml.pushTag("of");
	xml.addValue("strength", opticalFlow.getStrength());
	xml.addValue("offset", opticalFlow.getOffset());
	xml.addValue("lambda", opticalFlow.getLambda());
	xml.addValue("threshold", opticalFlow.getThreshold());
	xml.addValue("invertX", opticalFlow.getInverseX());
	xml.addValue("invertY", opticalFlow.getInverseY());
	xml.popTag();
    
	xml.popTag();
	xml.save(pth);
}

bool FlowManager::load(string pth) {
	ofxXmlSettings xml;
	if (xml.load(pth)) {
		if (xml.tagExists("flow")) {
			xml.pushTag("flow");
			
			if (xml.tagExists("particles")) {
				xml.pushTag("particles");
				particleFlow.activate((xml.getValue("active", 1) == 1));
				particleFlow.setSize(xml.getValue("size", particleFlow.getSize()));
				particleFlow.setSizeSpread(xml.getValue("sizeSpread", particleFlow.getSizeSpread()));
				particleFlow.setMass(xml.getValue("mass", particleFlow.getMass()));
				particleFlow.setMassSpread(xml.getValue("massSpread", particleFlow.getMassSpread()));
				particleFlow.setLifeSpan(xml.getValue("lifeSpan", particleFlow.getLifeSpan()));
				particleFlow.setLifeSpanSpread(xml.getValue("lifeSpanSpread", particleFlow.getLifeSpanSpread()));
				particleFlow.setBirthChance(xml.getValue("birthChance", particleFlow.getBirthChance()));
				particleFlow.setBirthVelocityChance(xml.getValue("birthVelocityChance", particleFlow.getBirthVelocityChance()));
				if (xml.tagExists("gravity")) {
					ofVec2f gg;
					gg.x = xml.getAttribute("gravity", "x", particleFlow.getGravity().x, 0);
					gg.y = xml.getAttribute("gravity", "y", particleFlow.getGravity().y, 0);
					particleFlow.setGravity(gg);
				}
				else {
					ofLogError("FlowManager::load", "no particle <gravity> in " + pth);
				}
				if (xml.tagExists("color")) {
					ofFloatColor cc;
					cc.r = xml.getAttribute("color", "r", particleFlow.getParticleColor().r, 0);
					cc.g = xml.getAttribute("color", "g", particleFlow.getParticleColor().g, 0);
					cc.b = xml.getAttribute("color", "b", particleFlow.getParticleColor().b, 0);
					cc.a = xml.getAttribute("color", "a", particleFlow.getParticleColor().a, 0);
					particleFlow.setParticleColor(cc);
				}
				else {
					ofLogError("FlowManager::load", "no particle <color> in " + pth);
				}
				xml.popTag();
			}
			else {
				ofLogError("FlowManager::load", "no <particles> in " + pth);
			}
            
			if (xml.tagExists("fluid")) {
				xml.pushTag("fluid");
                bFluid = (xml.getValue("active", 1)==1);
				fluid.setSpeed(xml.getValue("speed", fluid.getSpeed()));
				fluid.setCellSize(xml.getValue("cellSize", fluid.getCellSize()));
				fluid.setViscosity(xml.getValue("viscosity", fluid.getViscosity()));
				fluid.setVorticity(xml.getValue("vorticity", fluid.getVorticity()));
				fluid.setDissipation(xml.getValue("dissipation", fluid.getDissipation()));
				if (xml.tagExists("smoke")) {
					fluid.setSmokeSigma(xml.getAttribute("smoke", "sigma", fluid.getSmokeSigma(), 0));
					fluid.setSmokeWeight(xml.getAttribute("smoke", "weight", fluid.getSmokeWeight(), 0));
				}
				else {
					ofLogError("FlowManager::load", "no fluid <smoke> in " + pth);
				}
				if (xml.tagExists("gravity")) {
					ofVec2f gg;
					gg.x = xml.getAttribute("gravity", "x", fluid.getGravity().x, 0);
					gg.y = xml.getAttribute("gravity", "y", fluid.getGravity().y, 0);
					fluid.setGravity(gg);
				}
				else {
					ofLogError("FlowManager::load", "no fluid <gravity> in " + pth);
				}
				xml.popTag();
			}
			else {
				ofLogError("FlowManager::load", "no <fluid> in " + pth);
			}
            
			if (xml.tagExists("force")) {
				xml.pushTag("force");
				if (xml.tagExists("color")) {
					ofVec4f cc;
					cc.x = xml.getAttribute("color", "r", flexDrawForces[0].getForce().x, 0);
					cc.y = xml.getAttribute("color", "g", flexDrawForces[0].getForce().y, 0);
					cc.z = xml.getAttribute("color", "b", flexDrawForces[0].getForce().z, 0);
					cc.w = xml.getAttribute("color", "a", flexDrawForces[0].getForce().w, 0);
					flexDrawForces[0].setForce(cc);
				}
				else {
					ofLogError("FlowManager::load", "no force <color> in " + pth);
				}
				flexDrawForces[0].setStrength(xml.getValue("strength", flexDrawForces[0].getStrength()));
				flexDrawForces[0].setRadius(xml.getValue("radius", flexDrawForces[0].getRadius()));
				flexDrawForces[0].setEdge(xml.getValue("edge", flexDrawForces[0].getEdge()));
				xml.popTag();
			}
			else {
				ofLogError("FlowManager::load", "no <force> in " + pth);
			}
            
			if (xml.tagExists("of")) {
				xml.pushTag("of");
				opticalFlow.setStrength(xml.getValue("strength", opticalFlow.getStrength()));
				opticalFlow.setOffset(xml.getValue("offset", opticalFlow.getOffset()));
				opticalFlow.setLambda(xml.getValue("lambda", opticalFlow.getLambda()));
				opticalFlow.setThreshold(xml.getValue("threshold", opticalFlow.getThreshold()));
				opticalFlow.setInverseX((xml.getValue("invertX", opticalFlow.getInverseX()) == 1));
				opticalFlow.setInverseY((xml.getValue("invertY", opticalFlow.getInverseY()) == 1));
				xml.popTag();
			}
			else {
				ofLogError("FlowManager::load", "no <of> in " + pth);
			}
            
			xml.popTag();
		}
		else {
			ofLogError("FlowManager::load", "malformed " + pth);
			return false;
		}
	}
	else {
		ofLogError("FlowManager::load", "cannot load " + pth);
		return false;
	}
    
	return true;
}