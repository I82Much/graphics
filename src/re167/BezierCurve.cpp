#include "BezierCurve.h"
#include "Vector3.h"
#include "Matrix4.h"
#include <cmath>
using namespace RE167;

using std::cout;
using std::endl;

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

BezierCurve::BezierCurve(Vector3 points[], int numPoints)
{
    assert (numPoints >= 4);
    assert ((numPoints - 4) % 3 == 0 && "Piecewise cubic bezier curve must"\
        " have 3(n - 1) + 4 points; 4, 7, 10, ...");
    // 4, 7, 10, ...
    
    // 3(numCubicSegments − 1) + 4 = numPoints, 
    // do a little algebra and you get this.
    numCubicSegments = ((numPoints - 4) / 3) + 1;
    
    for (int i = 0; i < numPoints; i++) {
        controlPoints.push_back(points[i]);
    }
    createMatrices();
}



BezierCurve::~BezierCurve() {}


Vector3 BezierCurve::position(float t) const 
{
    // Curve is only defined for values of t between 0 and numCubicSegments
    assert(0.0f <= t && t <= numCubicSegments);

    int integer;
    float real;
    split(t, &integer, &real);
    
    int subcurveIndex = integer;

    // We're at the very end of our curve.
    if (subcurveIndex == numCubicSegments) {
        subcurveIndex -= 1;
        real += 1.0f;
    }
        
    Vector4 tVec(real*real*real, real*real, real, 1);
    Vector4 pos = matrices[subcurveIndex] * tVec;
    return Vector3(pos);
}

Vector3 BezierCurve::tangent(float t) const
{
    // Curve is only defined for values of t between 0 and 1
    assert(0.0f <= t && t <= numCubicSegments);
    
    int integer;
    float real;
    split(t, &integer, &real);
    
    int subcurveIndex = integer;
    
    // We're at the very end of our curve.
    if (subcurveIndex == numCubicSegments) {
        subcurveIndex -= 1;
        real += 1.0f;
    }

    Vector4 tVec(3*real*real, 2*real, 1, 0);
    Vector4 tangent = matrices[subcurveIndex] * tVec;
    return Vector3(tangent);
}

Vector3 BezierCurve::acceleration(float t) const
{
    // Curve is only defined for values of t between 0 and 1
    assert(0.0f <= t && t <= numCubicSegments);
    
    int integer;
    float real;
    split(t, &integer, &real);
    
    int subcurveIndex = integer;
    
    // We're at the very end of our curve.
    if (subcurveIndex == numCubicSegments) {
        subcurveIndex -= 1;
        real += 1.0f;
    }
    // Second derivative 
    Vector4 tVec(6*real, 2, 0, 0);
    Vector4 acceleration = matrices[subcurveIndex] * tVec;
    return Vector3(acceleration);
    
}

/**
* Given a floating point number, splits it into its integer part
* and real part, such that integerPart + realPart = real.
**/
void BezierCurve::split(float real, int *integerPart, float *realPart) {
    float integer;
    (*realPart) = modff(real, &integer);
    (*integerPart) = static_cast<int>(integer);
}



void BezierCurve::createMatrices() {
    
    // For each curve segment, determine the C matrix (G_Bez * WEIGHTS)
    for (int i = 0; i < numCubicSegments; i++) {
        // The curves are made of control points 0,1,2,3; 3,4,5,6...
        // Thus the starting segment is 3*i.
        int index = 3 * i;
        Vector3 P0 = controlPoints[index];
        Vector3 P1 = controlPoints[index+1];
        Vector3 P2 = controlPoints[index+2];
        Vector3 P3 = controlPoints[index+3];
        
        Matrix4 G_Bez(  P0.getX(), P1.getX(), P2.getX(), P3.getX(),
                        P0.getY(), P1.getY(), P2.getY(), P3.getY(),
                        P0.getZ(), P1.getZ(), P2.getZ(), P3.getZ(),
                        0,          0,          0,          0
            );
        Matrix4 C = G_Bez * WEIGHTS;
        matrices.push_back(C);
    }
}




/**
* Uniformly samples the curve and returns the points along the way
**/
std::vector<Vector3> BezierCurve::uniformPointSample(int numPoints) const {
    assert (numPoints >= 2);
    std::vector<Vector3> points;
    for (int i = 0; i < numPoints; i++) {
        // 0 <= t <= numCubicSegments
        float t =   static_cast<float>(numCubicSegments * i) / 
                    static_cast<float>(numPoints - 1);
        Vector3 point = position(t);
        points.push_back(point);
    }
    return points;
}

/**
* Uniformly samples the curve and returns the tangent vectors along the way
**/
std::vector<Vector3> BezierCurve::uniformTangentSample(int numPoints) const {
    assert (numPoints >= 2);
    std::vector<Vector3> tangents;
    for (int i = 0; i < numPoints; i++) {
        // 0 <= t <= numCubicSegments
        float t =   static_cast<float>(numCubicSegments * i) / 
                    static_cast<float>(numPoints - 1);
        Vector3 tangentVector = tangent(t);
        tangents.push_back(tangentVector);
    }
    return tangents;
}

/**
* Uniformly samples the curve and returns the tangent vectors along the way
**/
std::vector<Vector3> BezierCurve::uniformAccelerationSample(int numPoints) const {
    assert (numPoints >= 2);
    std::vector<Vector3> accelerationVectors;
    for (int i = 0; i < numPoints; i++) {
        // 0 <= t <= numCubicSegments
        float t =   static_cast<float>(numCubicSegments * i) / 
                    static_cast<float>(numPoints - 1);
        Vector3 accelerationVector = acceleration(t);
        accelerationVectors.push_back(accelerationVector);
    }
    return accelerationVectors;
}

// TODO: remove all this redundant stuff

void BezierCurve::test() {
    
    
    Vector3 p1(3,2,0);
    Vector3 p2(4,3,0);
    Vector3 p3(5,4,0);
    Vector3 p4(6,5,0);
    
    Vector3 expectedMidway(0.5f * (p1 + p4));
    
    BezierCurve b(p1,p2,p3,p4);

    Vector3 tangentAtZero = 3 * (p2 - p1);
    Vector3 tangentAtOne = 3 * (p4 - p3);
    
    Vector3 calculatedTangentAtZero = b.tangent(0);
    Vector3 calculatedTangentAtOne = b.tangent(1.0f);

    std::cout << "Tan0: " << tangentAtZero << "Tan0':" << calculatedTangentAtZero << std::endl;
    std::cout << "Tan1: " << tangentAtOne << "Tan1':" << calculatedTangentAtOne << std::endl;

    
    Vector3 startPos = b.position(0.0f);
    Vector3 midPoint = b.position(0.5f);
    
    Vector3 endPos = b.position(1.0f);
    cout << endPos << ", " << p4 << endl;

    
    assert(p1 == startPos);
    assert(p4 == endPos);
    assert(midPoint == expectedMidway);
    
    std::vector<Vector3> straightLine = b.uniformPointSample(2);
    std::vector<Vector3> tenPoints = b.uniformPointSample(10);
    
    std::vector<Vector3> tenTangentVectors = b.uniformTangentSample(10);
    
    
    std::cout << "Num points in straight line: " << straightLine.size() << std::endl;
    std::cout << "Num points in ten points: " << tenPoints.size() << std::endl;

    
    assert(straightLine.size() == 2);
    assert(tenPoints.size() == 10);
    
    assert(straightLine[0] == startPos);
    assert(straightLine[1] == endPos);
    
    assert(tenPoints[0] == startPos);
    assert(tenPoints[tenPoints.size() - 1] == endPos);
    
    
    
    std::vector<Vector3>::iterator i;
    for(i=straightLine.begin(); i != straightLine.end(); ++i) std::cout << (*i);
    std::cout << "\n\n" <<std::endl;
    for(i=tenPoints.begin(); i != tenPoints.end(); ++i) std::cout << (*i);
    
    
    
    for(i=tenTangentVectors.begin(); i != tenTangentVectors.end(); ++i) std::cout << (*i);
    
    Vector3 array[] = {p1,p2,p3,p4};
    BezierCurve fromArray(array, 4);
    
    assert(fromArray.numCubicSegments == 1);
    
    Vector3 array2[] = {p1,p2,p3,p4,p3,p2,p1};
    BezierCurve fromArray2(array2, 7);
    
    assert(fromArray2.numCubicSegments == 2);
}

