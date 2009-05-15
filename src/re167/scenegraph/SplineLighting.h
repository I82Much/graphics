/*
 *  SplineLighting.h
 *  
 *	A subclass of TransformGroup that contains a list of Torches.  Was created to implement the torches on the walls
 *
 *  Created by Susanna Kimport on 5/14/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __SplineLighting_h__
#define __SplineLighting_h__

#include "RE167_global.h"
#include "TransformGroup.h"
#include "Torch.h"
#include "Matrix4.h"
#include "../spline/Spline.h"
#include "Shader.h"

namespace RE167 {
	class RE167_EXPORT SplineLighting : public TransformGroup {
	public:
		static const int U = 0;
		static const int V = 1;
		static const int W = 2;
		
		SplineLighting(Spline* spline, int direction, float distance, int numSamples, int frequency, Shader* twoSpotTexture);
		~SplineLighting();
		
		bool addChild(Node* n) {return theLights->addChild(n);}
		bool removeChild(Node* n) {return theLights->removeChild(n);}
		Node* getChild(int i) {return theLights->getChild(i);}
		
		void draw(const Matrix4 &t, RenderContext *context, Camera * camera, bool cull) {
			theLights->draw(t,context,camera,cull);
		}
		
		void resetTransformation() { theLights->resetTransformation(); }
        inline void setTransformation(const Matrix4 &t) { theLights->setTransformation(t); }
        inline Matrix4 getTransformation() const { return theLights->getTransformation(); }
		
		void enableLight(int i);
		void disableLight(int i);
		bool inUse(int i);
		
	private:
		TransformGroup* theLights;
		int frequency;
	};
}

#endif