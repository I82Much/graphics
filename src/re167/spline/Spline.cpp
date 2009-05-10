#include "Spline.h"
#include "../Vector3.h"

using namespace RE167;

Spline::Spline() : transformation(Matrix4::IDENTITY) 
{
    
    
    
}

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
