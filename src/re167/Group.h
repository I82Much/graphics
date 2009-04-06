#ifndef GROUP_H_T5AP4XGM
#define GROUP_H_T5AP4XGM

#include "RE167_global.h"
#include "Node.h"
#include <list>

class Matrix4;

namespace RE167
{
	class RE167_EXPORT Group : public Node
	{
    public:
        /*
        virtual void draw();
        virtual void applyTransformation(const Matrix4 &t);
        */
        
        bool addChild(Node * n);
        bool removeChild(Node * n);
      
        static void test();
        
    protected:
        std::list<Node *> children;
        
    };
}




#endif /* end of include guard: GROUP_H_T5AP4XGM */
