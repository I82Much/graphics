/*
 *  Torch.h
 *  
 *
 *  Created by Susanna Kimport on 5/14/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __Torch_h__
#define __Torch_h__

#include "RE167_global.h"
#include "TransformGroup.h"
#include "Shape3D.h"
#include "LightNode.h"
#include "Vector3.h"
#include "Object.h"
#include "Shader.h"
#include "../spline/BezierCurve.h"
#include "GeometryFactory.h"

namespace RE167 {
	class RE167_EXPORT Torch : public TransformGroup {
	public:
		
		Torch(Vector3 location, Object* torchObject, TransformGroup* flameGroup, Shader* twoSpotTexture);
		~Torch();
		
		void enable() {light->enable();}
		void disable() {light->disable();}
		bool inUse() {return light->inUse();}
		
		void draw(const Matrix4 &t, RenderContext *context, Camera * camera, bool cull) {
			theTorch->draw(t,context,camera,cull);
		}
		
		void resetTransformation() { theTorch->resetTransformation(); }
        inline void setTransformation(const Matrix4 &t) { theTorch->setTransformation(t); }
        inline Matrix4 getTransformation() const { return theTorch->getTransformation(); }
		
		
	private:
		TransformGroup* theTorch;
		Shape3D* shape;
		TransformGroup* flame;
		LightNode* light;
		
	};
}

#endif