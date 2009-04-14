#ifndef GROUP_H_T5AP4XGM
#define GROUP_H_T5AP4XGM

#include "RE167_global.h"
#include "Node.h"
#include <list>


namespace RE167
{
	class RE167_EXPORT Group : public Node
	{
    public:
        
        bool addChild(Node * n);
        bool removeChild(Node * n);
        static void test();
        

        // TODO: make this private or protected
    public:
        std::list<Node *> children;
    };
}




#endif /* end of include guard: GROUP_H_T5AP4XGM */
