#ifndef LIGHTNODE_H_E0ZBHJQO
#define LIGHTNODE_H_E0ZBHJQO

#include "Leaf.h"
#include "Matrix4.h"

#include <iostream>

namespace RE167
{
    class Light;
    class Matrix4;
    class RenderContext;
    
	class RE167_EXPORT LightNode : public Leaf
	{
    public:
        
        LightNode(Light * aLight) : light(aLight) {
			spotDirection = light->getSpotDirection();
			position = light->getPosition();
			direction = light->getDirection();
			isInUse = true;
			transform = Matrix4::IDENTITY;
		}
        // Nothing allocated.
        ~LightNode() {}
        
        inline void setLight(Light * aLight) { light = aLight; }
        inline Light * getLight() { return light; }
        
        virtual void draw(const Matrix4 &t, RenderContext * context, Camera * camera, bool cull);
        
        inline void resetTransformation() { transform = Matrix4::IDENTITY; }
		void setTransformation(const Matrix4 &t);
        inline Matrix4 getTransformation() const { return transform; }
        
        static void test();
		
		void setSpotDirection(const Vector3& s);
		void setPosition(const Vector3& p);
		void setDirection(const Vector3& d);
		
		const Vector3& getSpotDirection () const {return spotDirection;}
		const Vector3& getPosition () const {return position;}
		
		void updateLight();
        
		// to allow some lights to be in use and others not to be in use
		void enable() {isInUse = true;}
		void disable() {isInUse = false;}
		bool inUse() {return isInUse;}
		
    protected:
        Light * light;
        Matrix4 transform;
		
		bool isInUse; // defaults to true
		
		// functionality of changing these values will be in the LightNode class, not GLRenderContext
		Vector3 spotDirection;
		Vector3 position;
		Vector3 direction;
    };
}


#endif /* end of include guard: LIGHTNODE_H_E0ZBHJQO */
