#include "BezierCurve.h"
#include "Vector3.h"
#include "Matrix4.h"
#include <cmath>
#include "BasicMath.h"
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
    BasicMath::split(t, integer, real);
    
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
    BasicMath::split(t, integer, real);
    
    int subcurveIndex = integer;
    
    // We're at the very end of our curve.
    if (subcurveIndex == numCubicSegments) {
        subcurveIndex -= 1;
        real += 1.0f;
    }

    Vector4 tVec(3*real*real, 2*real, 1, 0);
    Vector3 tangent = matrices[subcurveIndex] * tVec;
	
	// similarly to the code in PiecewiseSpline, since we do not enforce C1 continuity, we add the following
	if (1 - real < 0.1 && (subcurveIndex != numCubicSegments - 1)) {
		// we will take the average of this tangent, the tangent half as close to tPrime = 1 and the mirrored vectors in
		// in the next piece
		float nextT = real + ((1-real)/2.0);
		float firstTOnNext = (1-real)/2.0;
		float secondTOnNext = 1-real;
		
		Vector3 nextOnThisPiece = matrices[subcurveIndex] * Vector4(3*nextT*nextT, 2*nextT, 1, 0);
		Vector3 firstOnNextPiece = matrices[subcurveIndex + 1] * Vector4(3*firstTOnNext*firstTOnNext, 2*firstTOnNext, 1, 0);
		Vector3 secondOnNextPiece = matrices[subcurveIndex + 1] * Vector4(3*secondTOnNext*secondTOnNext, 2*secondTOnNext, 1, 0);
		
		// now we average them all
		tangent = 0.25 * (tangent + nextOnThisPiece + firstOnNextPiece + secondOnNextPiece);
		
	}
	
    return Vector3(tangent);
}

Vector3 BezierCurve::acceleration(float t) const
{
    // Curve is only defined for values of t between 0 and 1
    assert(0.0f <= t && t <= numCubicSegments);
    
    int integer;
    float real;
    BasicMath::split(t, integer, real);
    
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


void BezierCurve::createMatrices() {
    
    matrices.clear();
    
    // For each curve segment, determine the C matrix (G_Bez * WEIGHTS)
    for (int i = 0; i < numCubicSegments; i++) {
        // The curves are made of control points 0,1,2,3; 3,4,5,6...
        // Thus the starting segment is 3*i.
        int index = 3 * i;
        Vector3 P0 = transformation * Vector4::homogeneousPoint(controlPoints[index]);
        Vector3 P1 = transformation * Vector4::homogeneousPoint(controlPoints[index+1]);
        Vector3 P2 = transformation * Vector4::homogeneousPoint(controlPoints[index+2]);
        Vector3 P3 = transformation * Vector4::homogeneousPoint(controlPoints[index+3]);
        
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


/**
 Adaptive sampling for a spline:
 The first step is to compute the t-values to test - this is done by getTValues
 Next, calculate the position, tangent, and acceleration at each of those t-values - this is done by adaptiveSample
 **/
void BezierCurve::adaptiveSample(int numSamples, std::vector<Vector3>& positionVectors,
								 std::vector<Vector3>& tangentVectors, std::vector<Vector3>& accelerationVectors) const {
	
	std::vector<float> tValues = getTValues(numSamples);
	
	for (int i = 0; i < tValues.size(); i++) {
		float t = tValues[i];
		Vector3 point = position(t);
		Vector3 tangentVector = tangent(t);
		Vector3 accelerationVector = acceleration(t);
		
		positionVectors.push_back(point);
		tangentVectors.push_back(tangentVector);
		accelerationVectors.push_back(accelerationVector);
	}
	
} // end adaptiveSample

// to get the t-values, we first recursively divide the curve until we find a piece that is "flat enough"
// then we add the last control point of that piece to the tValues vector
// the numSamples parameter is used to dictate what the maximum step between tValues can be
std::vector<float> BezierCurve::getTValues(int numSamples) const {
	
	// first we need to figure out the maximum step size:
	float maxStepSize = static_cast<float>(numCubicSegments) / static_cast<float> (numSamples);
	
	// there is a tValue at 0
	std::vector<float> tValues;
	tValues.push_back(0.f);
	
//	std::cout << numCubicSegments << std::endl;
	
    for (int segment = 0; segment < numCubicSegments; segment++) {
        // The curves are made of control points 0,1,2,3; 3,4,5,6...
        // Thus the starting segment is 3*i.
        int index = 3 * segment;
        Vector3 p0 = transformation * Vector4::homogeneousPoint(controlPoints[index]);
        Vector3 p1 = transformation * Vector4::homogeneousPoint(controlPoints[index+1]);
        Vector3 p2 = transformation * Vector4::homogeneousPoint(controlPoints[index+2]);
        Vector3 p3 = transformation * Vector4::homogeneousPoint(controlPoints[index+3]);
        		
//		float scaleTValuesBy = 1.f / static_cast<float> (numCubicSegments);
		float startingTValue = static_cast<float> (segment);// / static_cast<float> (numCubicSegments);
		recursivelyDivide(maxStepSize, p0, p1, p2, p3, 1, startingTValue, 1, tValues);
		
    }
	
	while (numCubicSegments - tValues.back() > maxStepSize) {
		tValues.push_back(tValues.back() + maxStepSize);
	}
	tValues.push_back(numCubicSegments);
	return tValues;
	
} // end getTValues


// recursivelyDivide splits the given segment in two and adds tValues if the given piece is "flat enough"
void BezierCurve::recursivelyDivide (float maxStepSize, Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3,
									 float scaleTValuesBy, float startingTValue, int level,
									 std::vector<float>& tValues) const {
	
	if (flatEnough(p0,p1,p2,p3,0.001)) {
		float nextT = startingTValue + scaleTValuesBy*(pow(0.5, level));
		assert(nextT <= numCubicSegments);
		while (nextT - tValues.back() > maxStepSize) {
			tValues.push_back(tValues.back() + maxStepSize);
		}
		tValues.push_back(nextT);
		return;
	}
	
	// not flat enough so we divide the segment in two using de Casteljau algorithm
	Vector3 q0 = BasicMath::lerp(p0,p1,0.5);
	Vector3 q1 = BasicMath::lerp(p1,p2,0.5);
	Vector3 q2 = BasicMath::lerp(p2,p3,0.5);
	Vector3 r0 = BasicMath::lerp(q0,q1,0.5);
	Vector3 r1 = BasicMath::lerp(q1,q2,0.5);
	Vector3 point = BasicMath::lerp(r0,r1,0.5);
	
	recursivelyDivide(maxStepSize, p0, q0, r0, point, scaleTValuesBy, startingTValue, level + 1, tValues);
	recursivelyDivide(maxStepSize, point, r1, q2, p3, scaleTValuesBy,
					  startingTValue + pow(0.5,level)*scaleTValuesBy, level + 1, tValues);
	
} // end recursivelyDivide

// takes as input 4 points and a flatness parameter
// returns true if the distance from p1 and p2 to the line p0p3 is less than flatness
// idea from: http://mathforum.org/library/drmath/view/54731.html
bool BezierCurve::flatEnough(Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3, float flatness)  const {
	Vector3 p0p3 = p3 - p0;
	Vector3 p0p1 = p1 - p0;
	Vector3 p0p2 = p2 - p0;
	
	Vector3 cross0301 = p0p3.crossProduct(p0p1);
	Vector3 cross0302 = p0p3.crossProduct(p0p2);
	
	// now the length of these vectors is the area of the parallelogram with sides p0p3,p0p1 and p0p3,p0p2 respectively
	float area0301 = cross0301.magnitude();
	float area0302 = cross0302.magnitude();
	
	// now the distance from p1 to p0p3 is area0301 / p0p3.magnitude(); similarly for p2 to p0p3
	float distP1 = area0301 / p0p3.magnitude();
	float distP2 = area0302 / p0p3.magnitude();
	
	return (distP1 < flatness && distP2 < flatness);
	
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

