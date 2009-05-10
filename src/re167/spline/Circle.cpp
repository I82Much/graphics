#include "Circle.h"
#include <cmath>
#include "Vector3.h"
#include "../BasicMath.h"
#include "Vector4.h"

using namespace RE167;

Vector3 Circle::position(float t) const 
{
    assert( 0 <= t && t <= 1.0 );
    
    // Convert [0...1] to be [0..2PI]
    float tPrime = BasicMath::TWO_PI * t;
    Vector4 vec4Result = transformation * Vector4(cos(tPrime), sin(tPrime), 0 , 1);
    return Vector3(vec4Result);
    
}
Vector3 Circle::tangent(float t) const 
{
    assert( 0 <= t && t <= 1.0 );

    // Convert [0...1] to be [0..2PI]
    float tPrime = BasicMath::TWO_PI * t;
    // Derivative of cos is -sin
    // Derivative of sin is cos
    
    Vector4 vec4Result = transformation * Vector4(-sin(tPrime), cos(tPrime), 0, 0);
    return Vector3(vec4Result);
    
}

Vector3 Circle::acceleration(float t) const
{
    assert( 0 <= t && t <= 1.0 );
    
    // Convert [0...1] to be [0..2PI]
    float tPrime = BasicMath::TWO_PI * t;
    
    // Derivative of -sin is -cos
    // Derivative of cos is -sin
    Vector4 vec4Result = transformation * Vector4(-cos(tPrime), -sin(tPrime), 0, 0);
    return Vector3(vec4Result);
}