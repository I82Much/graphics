#ifndef NODE_H_NS9I4BNR
#define NODE_H_NS9I4BNR

#include "RE167_global.h"
#include "Matrix4.h"


namespace RE167
{
	class RE167_EXPORT Node
	{
    public:
        virtual void draw() = 0;
        
        virtual void applyTransformation(const Matrix4 &t) = 0;
        
    };
}

#endif /* end of include guard: NODE_H_NS9I4BNR */
