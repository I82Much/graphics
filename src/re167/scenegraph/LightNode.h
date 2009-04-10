#ifndef LIGHTNODE_H_E0ZBHJQO
#define LIGHTNODE_H_E0ZBHJQO

#include "Leaf.h"


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
        
        virtual void draw(const Matrix4 &t, RenderContext * context);
        
        static void test();
        
    protected:
        Light * light;
    };
}


#endif /* end of include guard: LIGHTNODE_H_E0ZBHJQO */
