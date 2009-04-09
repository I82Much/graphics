#include "Object.h"

using namespace RE167;

/**
* Using vertex data, calculates 
**/
void calculateBoundingSphere()
{
    float *vertices = getVertices();
    int numVertices = getNumVertices();
    Vector3 min, max;
    
    GeometryFactory::calculateBoundingBox(vertices, numVertices, min, max);
    Vector3 center = 0.5f * (min + max);

    sphereCenter(center.getX(), center.getY(), center.getZ())
    
}