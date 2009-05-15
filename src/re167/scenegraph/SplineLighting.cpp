/*
 *  SplineLighting.cpp
 *  
 *
 *  Created by Susanna Kimport on 5/14/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "SplineLighting.h"
#include "GeometryFactory.h"
#include "../spline/BezierCurve.h"
#include "Object.h"
#include "Basis.h"

using namespace RE167;

SplineLighting::SplineLighting(Spline* spline, int direction, float distance, int numSamples, int frequency, Shader* twoSpotTexture) {
	this->frequency = frequency;
	
	std::vector<Basis> referenceFrames = spline->getReferenceFrames(numSamples,true);
	theLights = new TransformGroup();
	
    // Make the curve that we will rotate around the y axis to create a
    // torch to place on the walls
    Vector3 torch0(.5,2,0);
    Vector3 torch1(.3,1.5,0);
    Vector3 torch2(.1,.1,0);
    Vector3 torch3(0,0,0);
    
    BezierCurve* torchCurve = new BezierCurve(torch0, torch1, torch2, torch3);
    Object* shape = GeometryFactory::createSurfaceOfRevolution(*torchCurve);
//	shape->setTransformation(Matrix4::scale(0.5,0.5,0.5));
	
	for (int i = 0; i < referenceFrames.size(); i++) {
		if (i % frequency == 0) {
			Vector3 loc = referenceFrames[i].getOrigin();
			Vector3 translateBy;
			switch(direction) {
				case U:
					translateBy = distance*referenceFrames[i].getU();
					break;
				case V:
					translateBy = distance*referenceFrames[i].getV();
					break;
				case W:
					translateBy = distance*referenceFrames[i].getW();
					break;
			}
			Matrix4 translation = Matrix4::translate(translateBy.getX(), translateBy.getY(), translateBy.getZ());
			loc = Vector3(translation*Vector4(loc));
			theLights->addChild(new Torch(loc, shape, twoSpotTexture));
			
			if (i > 3*frequency) {
				disableLight(i);
			}
		}
	}
	
}

SplineLighting::~SplineLighting() {
	delete theLights;
}

void SplineLighting::enableLight(int i) {
	Torch* child = dynamic_cast<Torch*> (theLights->getChild(i / frequency));
	child->enable();
}
void SplineLighting::disableLight(int i) {
	Torch* child = dynamic_cast<Torch*> (theLights->getChild(i / frequency));
	child->disable();
}
bool SplineLighting::inUse(int i) {
	Torch* child = dynamic_cast<Torch*> (theLights->getChild(i / frequency));
	return child->inUse();
}
