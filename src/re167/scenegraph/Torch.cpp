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
#include "Material.h"
#include "Texture.h"

using namespace RE167;

Torch::Torch(Vector3 location, Object* torchShape, TransformGroup* flameGroup, Shader* twoSpotTexture) {
	
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
	
	shape = new Shape3D(torchShape);
	theTorch->addChild(shape);
	
	flame = flameGroup;
	flame->setTransformation(Matrix4::translate(0,3.0+0.25,0));
	theTorch->addChild(flame);
	
	
} // end constructor

Torch::~Torch () {
	delete theTorch;
}