#include "BezierCurve.h"
#include "Vector3.h"
#include "Matrix4.h"
using namespace RE167;


// These represent the coefficients for the Bernstein polynomials
const Matrix4 BezierCurve::WEIGHTS(-1, 3, -3, 1,
                                3, -6, 3, 0,
                                -3, 3, 0, 0,
                                1, 0, 0, 0);




BezierCurve::BezierCurve(const Vector3 &p0, 
                        const Vector3 &p1,
                        const Vector3 &p2,
                        const Vector3 &p3)
{
    controlPoints.push_back(p0);
    controlPoints.push_back(p1);
    controlPoints.push_back(p2);
    controlPoints.push_back(p3);
    numCubicSegments = 1;
    createMatrices();
}

BezierCurve::~BezierCurve() {}
                            

Vector3 BezierCurve::position(float t) {
    // Curve is only defined for values of t between 0 and 1
    assert(0.0f <= t && t <= 1.0f);
    
    int subcurveIndex = calculateIndex(t);



    Vector4 tVec(t*t*t, t*t, t, 1);
    Vector4 pos = matrices[subcurveIndex] * tVec;
    return Vector3(pos.getX(), pos.getY(), pos.getZ());
}

Vector3 BezierCurve::tangent(float t) 
{
    // Curve is only defined for values of t between 0 and 1
    assert(0.0f <= t && t <= 1.0f);

    return Vector3();
}

// TODO: fix this
int BezierCurve::calculateIndex(float t) 
{
    return 0;
}

// TODO: precompute the C matrices
void BezierCurve::createMatrices() {
    for (int i = 0; i < numCubicSegments; i++) {
        
    }
}

void BezierCurve::test() {
    /*
    vase:
    (80,20),
    (90,40),
    (80,60),
    (50,120),
    (80,180)*/
}