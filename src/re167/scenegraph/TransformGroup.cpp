#include "TransformGroup.h"
#include <iostream>
#include <list>


using namespace RE167;
class Camera;

TransformGroup::~TransformGroup() {}

TransformGroup::TransformGroup(const Matrix4 &t) : transform(t) {}

/**
*
**/
void TransformGroup::draw(const Matrix4 &t, RE167::RenderContext *context, Camera * camera) 
{
    Matrix4 t_new = t * transform;
    // For each node in the list, apply the transform to them and continue
    for (std::list<Node *>::iterator i = children.begin(); i != children.end(); i++) 
    {
        (*i)->draw(t_new, context, camera);
    }
}

