#ifndef LIGHTNODE_H_E0ZBHJQO
#define LIGHTNODE_H_E0ZBHJQO

#include "Leaf.h"
#include "Matrix4.h"

namespace RE167
{
    class Light;
    class Matrix4;
    class RenderContext;
    
	class RE167_EXPORT LightNode : public Leaf
	{
    public:
        
        LightNode(Light * aLight) : light(aLight) {}
        // Nothing allocated.
        ~LightNode() {}
        
        inline void setLight(Light * aLight) { light = aLight; }
        inline Light * getLight() { return light; }
        
        virtual void draw(const Matrix4 &t, RenderContext * context, Camera * camera, bool cull);
        
        inline void resetTransformation() { transform = Matrix4::IDENTITY; }
        inline void setTransformation(const Matrix4 &t) { transform = t; }
        inline Matrix4 getTransformation() const { return transform; }
        
        static void test();
        
    protected:
        Light * light;
        Matrix4 transform;
    };
}


#endif /* end of include guard: LIGHTNODE_H_E0ZBHJQO */
