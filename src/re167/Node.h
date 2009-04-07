#ifndef NODE_H_NS9I4BNR
#define NODE_H_NS9I4BNR

#include "RE167_global.h"
#include "Matrix4.h"


namespace RE167
{
	class RE167_EXPORT Node
	{
    public:
        virtual void draw(const Matrix4 & t) = 0;
        
        inline Matrix4 getLocalToWorldTransform() { return localToWorldTransform; }
        inline void setLocalToWorldTransform(const Matrix4 &t) { localToWorldTransform = t; }
        
    protected:
        Matrix4 localToWorldTransform;
    };
}

#endif /* end of include guard: NODE_H_NS9I4BNR */
