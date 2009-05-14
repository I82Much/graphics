#include "Group.h"
#include <algorithm>
#include <iostream>
#include "TransformGroup.h"

/**
* @author Nicholas Dunn
* @date   2009-04-05
**/

using namespace RE167;


/**
* Adds the given node to children list.  Note that an attempt to add
* own pointer to list will fail
**/
bool Group::addChild(Node * n) 
{ 
    if (n == this) {
        return false;
    }
    else {
        children.push_front(n);
        return true;
    }
}

/**
* Removes the given node from the children list.  Returns false if 
* Node not in the list
**/
bool Group::removeChild(Node * n) 
{
    std::list<Node *>::iterator location;
    location = std::remove(children.begin(), children.end(), n);
    // Node is not in the list
    if (location == children.end()) {
        return false;
    }
    // We found it, remove it and return true
    else {
        children.erase(location, children.end());
        return true;
    }
    
}

// get child that was the ith child added to children (0 based)
Node* Group::getChild (int i) {
	i = children.size() - 1 - i;
	int counter = 0;
	for (std::list<Node*>::iterator it = children.begin(); it != children.end(); it++) {
		if (counter == i) {
			return *it;
		}
		counter++;
	}
	std::cout << "I should so not be here" << std::endl;
	return NULL;
}


/*
void Group::draw() {}

void Group::applyTransformation(const Matrix4 &t) {}
*/

void Group::test() 
{
    std::cout << "Testing functionality of Group" << std::endl;
    Group * a = new TransformGroup();
    Group * b = new TransformGroup();
    Group * c = new TransformGroup();
    Group * null = NULL;
    
    std::cout << "Adding children" << std::endl;
    bool addSelf = a->addChild(a);
    bool addB = a->addChild(b);
    bool addC = a->addChild(c);

    assert (!addSelf);
    assert(addB);
    assert(addC);
    
    assert(a->children.size() == 2);
    
    std::cout << "Removing children" << std::endl;
    bool removeSelf = a->removeChild(a);
    bool removeB = a->removeChild(b);
    bool removeC = a->removeChild(c);
    bool removeNull = a->removeChild(null);
    
    assert(!removeSelf);
    assert(removeB);
    assert(removeC);
    assert(!removeNull);
    
    assert(a->children.empty());
    
    delete a;
    delete b;
    delete c;
    
    
}
    