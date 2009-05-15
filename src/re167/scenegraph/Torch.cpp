/*
 *  Torch.cpp
 *  
 *
 *  Created by Susanna Kimport on 5/14/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Torch.h"
#include "Light.h"
#include "Object.h"

using namespace RE167;

Torch::Torch(Vector3 location, Object* o, Shader* twoSpotTexture) {
	
	theTorch = new TransformGroup();
	theTorch->setTransformation(Matrix4::translate(location.getX(), location.getY(), location.getZ()));
	
	Light* theLight = new Light();
	theLight->setType(Light::POINT);
    theLight->setDiffuseColor(Vector3(1,1,1));
    theLight->setAmbientColor(Vector3(.2,.2,.2));
    theLight->setSpecularColor(Vector3(1,1,1));
	theLight->setPosition(Vector3(0,0,0));
	
	light = new LightNode(theLight);
	theTorch->addChild(light);
	
	shape = new Shape3D(o);
	theTorch->addChild(shape);
	
	flame = new TransformGroup();
	Shape3D* flameShape = new Shape3D(Geometry::surfaceOfRev(PreMadeCurve::candleFlame(),50,50,false));
	Material* mFlame = new Material(Material::OPEN_GL_DEFAULT);
//	mFlame->setDiffuse(Vector3(1,1,1));
//	mFlame->setAmbient(Vector3(1,1,1));
//	mFlame->setSpecular(Vector3(1,1,1));
	Texture *texFlame = new Texture(new QImage("images/flames.jpg"));
	mFlame->setTexture(texFlame);
	mFlame->setShader(twoSpotTexture);
	flameShape->setMaterial(mFlame);
	flame->addChild(flameShape);
	flame->setTransformation(Matrix4::translation(0,1.0+0.25,0)*Matrix4::uniformScale(0.4));
	theTorch->addChild(flame);
	
	
} // end constructor

Torch::~Torch () {
	delete theTorch;
}