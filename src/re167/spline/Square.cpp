#include "Square.h"
#include "Vector4.h"
#include "Vector3.h"

using namespace RE167;

const Vector3 Square::UPPER_LEFT(-1,1,0);
const Vector3 Square::UPPER_RIGHT(1,1,0);
const Vector3 Square::LOWER_RIGHT(1,-1,0);
const Vector3 Square::LOWER_LEFT(-1,-1,0);

const Vector3 Square::UPPER_TANGENT(1,0,0);
const Vector3 Square::RIGHT_TANGENT(0,-1,0);
const Vector3 Square::LOWER_TANGENT(-1,0,0);
const Vector3 Square::LEFT_TANGENT(0,1,0);

const int Square::TOP(0);
const int Square::RIGHT(1);
const int Square::BOTTOM(2);
const int Square::LEFT(3);


/**
* Represents a unit square in the xy plane centered at the origin.  Square is
* drawn clockwise in order top, right, bottom, left
**/


Vector3 Square::position(float t) const
{
    int side;
    float tPrime;
    
    Vector3 result;
    
    translateT(t, tPrime, side);
    switch (side) {
        case TOP:
        result = BasicMath::lerp(UPPER_LEFT, UPPER_RIGHT, tPrime);
        break;

        case RIGHT:
        result = BasicMath::lerp(UPPER_RIGHT, LOWER_RIGHT, tPrime);
        break;
        
        case BOTTOM:
        result = BasicMath::lerp(LOWER_RIGHT, LOWER_LEFT, tPrime);
        break;
        
        case LEFT:
        result = BasicMath::lerp(LOWER_LEFT, UPPER_LEFT, tPrime);
        break;
        default:
            assert(false && "Error in calculating position in Square.cpp");
    }
    return Vector3(transformation * Vector4::homogeneousPoint(result));
    
    
}
Vector3 Square::tangent(float t) const
{    
    int side;
    float tPrime;
    translateT(t, tPrime, side);
    switch (side) {
        case TOP:
        return Vector3(transformation * Vector4::homogeneousVector(UPPER_TANGENT));
        case RIGHT:
        return Vector3(transformation * Vector4::homogeneousVector(RIGHT_TANGENT));
        case BOTTOM:
        return Vector3(transformation * Vector4::homogeneousVector(LOWER_TANGENT));
        case LEFT:
        return Vector3(transformation * Vector4::homogeneousVector(LEFT_TANGENT));
        default:
            assert(false && "Error in calculating position in Square.cpp");
    }
    
}
Vector3 Square::acceleration(float t) const
{
    // There is no acceleration for a square - each side is a straight line
    return Vector3(0,0,0);
}

/**
* Given a value in the range [0..1], translates into the corresponding t value
* for the side it corresponds with.  In other words, if t is between 0 and .25,
* it corresponds with the top side; scale the t to work with the segment of the
* line rather than the whole curve.
**/
void Square::translateT(float t, float &tPrime, int &side) const
{
    assert(0 <= t && t <= 1.0);
    if (0 <= t && t <= .25) {
        // Scale [0, .25] to be [0, 1]
        tPrime = t * 4.0;
        side = TOP;
    }
    // Right
    else if (t <= .5) {
        // Scale (.25, .5] to [0, 1]
        tPrime = (t - .25) * 4.0;
        side = RIGHT;
    }
    // Bottom
    else if (t <= .75) {
        // Scale (.5, .75] to [0, 1]
        tPrime = (t - .5) * 4.0;
        side = BOTTOM;
    }
    // Left
    else {
        // Scale (.75, 1] to [0, 1]
        tPrime = (t - .75) * 4.0;
        side = LEFT;
    }
}


