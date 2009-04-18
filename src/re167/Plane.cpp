#include "Plane.h"
#include <list>
#include <map>



using namespace RE167;

// Any plane can be defined by 3 points on it.  The normal will be defined
// by the right hand rule.  Points should be passed in in counterclockwise
// order for the normal to point in correct direction.
Plane::Plane(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3) {
    Vector3 normal = (p2 - p1).crossProduct(p3 - p1).normalize();
    
    unitNormal = Vector4(normal.getX(), normal.getY(), normal.getZ(), 0);
    distance = (p1.dotProduct(normal));
}



/**
* Calculates the signed perpendicular distance from the point x to the plane.
* The distance is the length of the projection of (x-p) onto n.
* A distance of 0 indicates that the point is on the plane, a positive 
* number indicates that it is on the side of the plane that the normal points 
* to, and negative on the opposite side.
**/
float Plane::distanceTo(const Vector4 &x) 
{
    return x.dotProduct(unitNormal) - distance;
}


void Plane::test() 
{
    // xz plane
    Plane xz(0, Vector4(0,1,0,0) );
    
    //A bunch of points we know that lie on the plane
    std::list<Vector4> pointsOnXZPlane;
    pointsOnXZPlane.push_back(Vector4(100,0,3025,1));
    pointsOnXZPlane.push_back(Vector4(0,0,0,1));
    pointsOnXZPlane.push_back(Vector4(-15,0,-41,1));
    
    for (std::list<Vector4>::iterator i = pointsOnXZPlane.begin(); i != pointsOnXZPlane.end(); i++) 
    {
        assert(xz.distanceTo(*i) == 0.0f);
    }
    
    std::map<Vector4, float> pointsNotOnXZPlane;
    pointsNotOnXZPlane[Vector4(0,1,0,1)] = 1.0f;
    pointsNotOnXZPlane[Vector4(0,-1,0,1)] = -1.0f;
    pointsNotOnXZPlane[Vector4(1235,-1,    4125,1)] = -1.0f;
    pointsNotOnXZPlane[Vector4(1235,-1.125,4125,1)] = -1.125f;
    int counter = 0;
    for(std::map<Vector4, float>::const_iterator it = pointsNotOnXZPlane.begin(); it != pointsNotOnXZPlane.end(); ++it)
    {
        
        std::cout << it->first << " distance: " << it->second << std::endl;
        assert(xz.distanceTo(it->first) == it->second);
        counter++;
    }

    std::cout << "size of map:" << pointsNotOnXZPlane.size() << std::endl;
    std::cout << "Number of iteratios: " << counter << std::endl;
    
    
    
    
}