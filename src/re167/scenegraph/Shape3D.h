#ifndef SHAPE3D_H_6ZKDY82T
#define SHAPE3D_H_6ZKDY82T

#include "RE167_global.h"
#include "Leaf.h"

class Object;


namespace RE167
{
    class Camera;
    
	class RE167_EXPORT Shape3D : public Leaf
	{
    public:
        
        
        Shape3D(Object * s, Material * m = NULL) : shape(s), material(m) {}
        // Nothing allocated.
        virtual ~Shape3D() {}
        
        inline void setObject(Object * s) { shape = s; }
        inline Object * getObject() { return shape; }
        
        inline void setMaterial(Material * m) { material = m; }
        inline Material * getMaterial() { return material; }
        
        virtual void draw(const Matrix4 &t, RenderContext * context, Camera * camera, bool cull);
        
        static void test();
        
    protected:
        Object * shape;
        Material * material;
    };
}




#endif /* end of include guard: SHAPE3D_H_6ZKDY82T */
