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
#include "Texture.h"
#include "Material.h"

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
	Material* mShape = new Material();
	mShape->setDiffuse(Vector3(1,1,1));
	mShape->setAmbient(Vector3(1,1,1));
	mShape->setSpecular(Vector3(1,1,1));
	Texture *texShape = new Texture(new QImage("images/woodPanel.jpg", "jpg"));
	mShape->setTexture(texShape);
	mShape->setShader(twoSpotTexture);
	shape->setMaterial(mShape);
	
//	shape->setTransformation(Matrix4::scale(0.5,0.5,0.5));
	
	TransformGroup* flame = new TransformGroup();
	Vector3 f1(0.01,0 * 0.2,0);
	Vector3 f2(0.03, -0.3 * 0.2, 0);
	Vector3 f3(0.05, -0.5 * 0.2, 0);
	Vector3 f4(0.07, -0.6 * 0.2, 0);
	Vector3 f5(0.1, -1 * 0.2, 0);
	Vector3 f6(0.1, -1.3 * 0.2, 0);
	Vector3 f7(0.07, -1.9 * 0.2, 0);
	Vector3 f8(0.05, -2.0 * 0.2, 0);
	Vector3 f9(0.03, -2.2 * 0.2, 0);
	Vector3 f10(0.01, -2.5 * 0.2, 0);
	
	Vector3 flameArray[] = {f1,f2,f3,f4,f5,f6,f7,f8,f9,f10};
	BezierCurve* flameCurve = new BezierCurve(flameArray, sizeof(flameArray) / sizeof(Vector3));
	flameCurve->setTransformation(Matrix4::scale(3,3,3));
	
	
	
	Object* flameObj = GeometryFactory::createSurfaceOfRevolution(*flameCurve,50,50);
	Material* mFlame = new Material();
	mFlame->setDiffuse(Vector3(1,1,1));
	mFlame->setAmbient(Vector3(1,1,1));
	mFlame->setSpecular(Vector3(1,1,1));
	Texture *texFlame = new Texture(new QImage("images/flames.jpg", "jpg"));
	mFlame->setTexture(texFlame);
	mFlame->setShader(twoSpotTexture);
	flameObj->setMaterial(mFlame);
	Shape3D* flameShape = new Shape3D(flameObj);
	flame->addChild(flameShape);
	
	
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
			Torch* nextTorch = new Torch(loc, shape, flame, twoSpotTexture);
			nextTorch->disable();
			theLights->addChild(nextTorch);
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
