#ifndef TRANSFORMGROUP_H_CZHOTBTJ
#define TRANSFORMGROUP_H_CZHOTBTJ

#include "RE167_global.h"
#include "Group.h"
#include "Node.h"
#include "Matrix4.h"


namespace RE167
{
	class RE167_EXPORT TransformGroup : public Group
	{
    public:
        
        
        TransformGroup(const Matrix4 &t = Matrix4::IDENTITY);
        virtual ~TransformGroup();
        
        
        virtual void draw(const Matrix4 &t, RenderContext *context);
        
        static void test();
        
        inline void resetTransformation() { transform = Matrix4::IDENTITY; }
        inline void setTransformation(const Matrix4 &t) { transform = t; }
        inline Matrix4 getTransformation() const { return transform; }
 
    protected:
        Matrix4 transform;
        
    };
}




#endif /* end of include guard: TRANSFORMGROUP_H_CZHOTBTJ */
