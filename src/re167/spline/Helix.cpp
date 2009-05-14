#include "Helix.h"
#include <cmath>
#include "Vector3.h"
#include "../BasicMath.h"
#include "Vector4.h"

#include "BezierCurve.h"
#include "Circle.h"

using namespace RE167;

Helix::Helix(float numTurns_) : numTurns(numTurns_) 
{

    Vector3 s1(1,1,1);
    Vector3 s2(2,2,1);
    Vector3 s3(2,2,1);
    Vector3 s4(1,1,1);
    
    scaleCurve = new BezierCurve(s1,s2,s3,s4);
    
    //scaleCurve = new Circle();
}

Helix::~Helix()
{
    delete scaleCurve;
}



Vector3 Helix::position(float t) const 
{
    assert( 0 <= t && t <= 1.0 );
    
    // Convert [0...1] to be [0..2PI]
    float tPrime = BasicMath::TWO_PI * t * numTurns;
    
    Vector3 scale = scaleCurve->position(t);
    float xScale = scale.getX();
    float yScale = scale.getY();
    float zScale = scale.getZ();
    Matrix4 scaleMatrix = Matrix4::scale(xScale, yScale, zScale);
    
    Vector4 vec4Result = transformation * scaleMatrix * Vector4(cos(tPrime), sin(tPrime), t , 1);
    return Vector3(vec4Result);
    
}
Vector3 Helix::tangent(float t) const 
{
    assert( 0 <= t && t <= 1.0 );

    // Convert [0...1] to be [0..2PI]
    float tPrime = BasicMath::TWO_PI * t * numTurns; 
    // Derivative of cos is -sin
    // Derivative of sin is cos

    Vector3 scale = scaleCurve->position(t);
    float xScale = scale.getX();
    float yScale = scale.getY();
    float zScale = scale.getZ();
    Matrix4 scaleMatrix = Matrix4::scale(xScale, yScale, zScale);
    

    
    Vector4 vec4Result = transformation * scaleMatrix * Vector4(-sin(tPrime), cos(tPrime), 1, 0);
    return Vector3(vec4Result);
}

Vector3 Helix::acceleration(float t) const
{
    assert( 0 <= t && t <= 1.0 );
    
    // Convert [0...1] to be [0..2PI]
    float tPrime = BasicMath::TWO_PI * t * numTurns;
    
    
    Vector3 scale = scaleCurve->position(t);
    float xScale = scale.getX();
    float yScale = scale.getY();
    float zScale = scale.getZ();
    Matrix4 scaleMatrix = Matrix4::scale(xScale, yScale, zScale);
    
    
    // Derivative of -sin is -cos
    // Derivative of cos is -sin
    Vector4 vec4Result = transformation * scaleMatrix * Vector4(-cos(tPrime), -sin(tPrime), 0, 0);
    return Vector3(vec4Result);
}

// since a helix is equally curvey at all points, adaptive sampling a circle is the same as uniformly sampling it
void Helix::adaptiveSample(int numSamples, std::vector<Vector3>& position,
						   std::vector<Vector3>& tangent, std::vector<Vector3>& acceleration) const {
	
	position = uniformPointSample(numSamples);
	tangent = uniformTangentSample(numSamples);
	acceleration = uniformAccelerationSample(numSamples);
	
}
