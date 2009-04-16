#ifndef NODE_H_NS9I4BNR
#define NODE_H_NS9I4BNR

#include "RE167_global.h"
#include "Matrix4.h"
#include "RenderContext.h"

namespace RE167
{
	class RE167_EXPORT Node
	{
    public:
        virtual ~Node() {}
        virtual void draw(const Matrix4 & t, RenderContext * context) = 0;
    };
}

#endif /* end of include guard: NODE_H_NS9I4BNR */
