#ifndef SHAPE3D_H_6ZKDY82T
#define SHAPE3D_H_6ZKDY82T

#include "RE167_global.h"
#include "Leaf.h"

class Object;


namespace RE167
{
	class RE167_EXPORT Shape3D : public Leaf
	{
    public:
        
        Shape3D(Object * s) : shape(s) {}
        // Nothing allocated.
        ~Shape3D() {}
        
        inline void setObject(Object * s) { shape = s; }
        inline Object * getObject() { return shape; }
        
        virtual void draw();
        
        virtual void applyTransformation(const Matrix4 &t);
        
        static void test();
        
    protected:
        Object * shape;
        Matrix4 transformation;
    };
}




#endif /* end of include guard: SHAPE3D_H_6ZKDY82T */
