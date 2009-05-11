#include "Spline.h"
#include "../Vector3.h"
#include "../Basis.h"

/**
* A spline represents any ``curve'' in 3-space; in other words a function
* of some parameter t that returns a position in space.
* The curves also have support for calculating the tangent vector to the
* curve, as well as the acceleration vector (the first and second derivative of
* the position function, respectively)
* @author Nicholas Dunn
**/

using namespace RE167;

Spline::Spline() : transformation(Matrix4::IDENTITY) {}

std::vector <Vector3> Spline::uniformPointSample(int numPoints) const
{
    assert (numPoints >= 2);
    std::vector<Vector3> points;
    for (int i = 0; i < numPoints; i++) {
        // 0 <= t <= numCubicSegments
        float t =   static_cast<float>(i) / 
                    static_cast<float>(numPoints - 1);
        Vector3 point = position(t);
        points.push_back(point);
    }
    return points;
}
std::vector <Vector3> Spline::uniformTangentSample(int numPoints) const
{
    assert (numPoints >= 2);
    std::vector<Vector3> points;
    for (int i = 0; i < numPoints; i++) {
        // 0 <= t <= numCubicSegments
        float t =   static_cast<float>(i) / 
                    static_cast<float>(numPoints - 1);
        Vector3 point = tangent(t);
        points.push_back(point);
    }
    return points;
}
std::vector <Vector3> Spline::uniformAccelerationSample(int numPoints) const
{
    assert (numPoints >= 2);
    std::vector<Vector3> points;
    for (int i = 0; i < numPoints; i++) {
        // 0 <= t <= numCubicSegments
        float t =   static_cast<float>(i) / 
                    static_cast<float>(numPoints - 1);
        Vector3 point = acceleration(t);
        points.push_back(point);
    }
    return points;
}

/**
* Calculates <code>numSamples</code> reference frames along the curve. In 
* other words, a local coordinate system at uniformly sampled points along
* the spline, with associated tangent vector, normal vector, and binormal
* vector.
* 
* Note that there is not a unique local coordinate system at each point along
* the curve; rather this is chosen to minimize rotation/torsion in adjacent
* reference frames.
*
* Makes heavy use of the paper "Calculation of Reference Frames along a Space
* Curve" by Jules Bloomenthal, published in Graphics Gems, vol.1.
*
* These reference frames are used in two different ways.  
* Firstly, they can be used to extract one spline along another spline's path;
* see GeometryFactory::createLoft for this usage.
*
* The second usage is to allow a camera to follow a path; the camera can simply
* use the tangent vector as a lookat vector, and then use the normal vector as
* the Up vector.
**/
std::vector <Basis> Spline::getReferenceFrames(int numSamples) const
{
    
}
