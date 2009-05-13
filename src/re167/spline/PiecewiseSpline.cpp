#include "PiecewiseSpline.h"
#include "Vector3.h"
#include "Vector4.h"


#include "Square.h"
#include "Circle.h"

using namespace RE167;

/**
* A piecewise spline simply holds a list of splines which it then uses to 
* calculate positions, tangents, and accelerations.  Since a curve can be
* seen as x(t), y(t), z(t), we let each of the x(t), y(t), and z(t) be 
* stepwise functions.  For instance, if we have two children, from 0 <= t <= .5
* we use the first curve, and from .5 < t <= 1 we use the second curve.
**/


PiecewiseSpline::PiecewiseSpline(Spline * first)
{
    addSpline(first);
}
PiecewiseSpline::~PiecewiseSpline()
{
    
}

Vector3 PiecewiseSpline::position(float t) const
{
    int curveIndex;
    float tPrime;
    calculateCurve(t, curveIndex, tPrime);
    
    Vector3 result = pieces[curveIndex]->position(tPrime);
    
    return Vector3(transformation * Vector4::homogeneousVector(result));
}
Vector3 PiecewiseSpline::tangent(float t) const {
	// Since we don't enforce C1 continuity of the PiecewiseSpline, we need to average tangents approaching the breakpoints
	
    int curveIndex;
    float tPrime;
    calculateCurve(t, curveIndex, tPrime);

    Vector3 result = pieces[curveIndex]->tangent(tPrime);
	
	// now, we have to determine if we are approaching a breakpoint
	// first, we must determine how many pieces there are and make sure we're not on the last piece
	int numCurves = static_cast<int>(pieces.size());
	bool onLast = (numCurves == curveIndex + 1);
	// tPrime is between 0 and 1 so we will see if it is "close enough" to 1 (if we are on the last piece, we do nothing)
	if (1 - tPrime < 0.01 && !onLast) {
		// we will take the average of this tangent, the tangent half as close to tPrime = 1 and the mirrored vectors in
		// in the next piece
		Vector3 nextOnThisPiece = pieces[curveIndex]->tangent(tPrime + ((1-tPrime)/2.0));
		Vector3 firstOnNextPiece = pieces[curveIndex + 1]->tangent((1-tPrime)/2.0);
		Vector3 secondOnNextPiece = pieces[curveIndex + 1]->tangent(1-tPrime);
		
		// now we average them all
		result = 0.25 * (result + nextOnThisPiece + firstOnNextPiece + secondOnNextPiece);
	}

    return Vector3(transformation * Vector4::homogeneousVector(result));
}
Vector3 PiecewiseSpline::acceleration(float t) const
{
    int curveIndex;
    float tPrime;
    calculateCurve(t, curveIndex, tPrime);

    Vector3 result = pieces[curveIndex]->acceleration(tPrime);
    
    return Vector3(transformation * Vector4::homogeneousVector(result));
}


/**
* Adds the given node to children list.  Note that an attempt to add
* own pointer to list will fail
**/
bool PiecewiseSpline::addSpline(Spline * n) 
{ 
    if (n == this) {
        return false;
    }
    else {
        pieces.push_back(n);
        return true;
    }
    
    
    
}

/**
* Removes the given node from the children list.  Returns false if 
* Node not in the list
**/
bool PiecewiseSpline::removeSpline(Spline * n) 
{
    std::vector<Spline *>::iterator location;
    location = std::remove(pieces.begin(), pieces.end(), n);
    // Node is not in the list
    if (location == pieces.end()) {
        return false;
    }
    // We found it, remove it and return true
    else {
        pieces.erase(location, pieces.end());
        return true;
    }
}


/**
* Given a value of t between 0 and 1, determines which curve corresponds to this
* value of t, and then what a transformed t is for this curve.
**/
void PiecewiseSpline::calculateCurve(const float t, int &curveIndex, float &tPrime) const
{
    assert (0 <= t && t <= 1.0f);
    
    int numCurves = static_cast<int>(pieces.size());
    // e.g. if there are two subcurves, than each has .5 of the total 
    // range of our t parameter
    float sizeOfCurve = 1.0f / static_cast<float> (numCurves);
    
    curveIndex = 0;
    float tCopy = t;
    
    for (int i = 0; i < numCurves; i++) 
    {
        // for instance, when we have two pieces, if it's less than 1 * .5,
        // it's index 0; otherwise it's less than 2 * .5 and index 1
        if (tCopy <= (i + 1) * sizeOfCurve) {
            curveIndex = i;
            break;
        }
    }
    
    float leftoverT = t - (curveIndex * sizeOfCurve);
    tPrime = leftoverT / sizeOfCurve; 
}


void PiecewiseSpline::test()
{
    Circle circle;
    Square square;
    PiecewiseSpline piece(&circle);
    
    // It has one element
    assert(piece.numSplines() == 1);
    
    int curveIndex;
    float tPrime;
    
    piece.calculateCurve(0, curveIndex, tPrime);
    assert(curveIndex == 0);
    assert(tPrime == 0);
    
    piece.calculateCurve(.5, curveIndex, tPrime);
    assert(curveIndex == 0);
    assert(tPrime == .5);
    
    piece.calculateCurve(1, curveIndex, tPrime);
    assert(curveIndex == 0);
    assert(tPrime == 1);
    
    bool addedSquare = piece.addSpline(&square);
    assert(addedSquare);

    piece.calculateCurve(0, curveIndex, tPrime);
    assert(curveIndex == 0);
    assert(tPrime == 0);

    piece.calculateCurve(.75, curveIndex, tPrime);
    assert(curveIndex == 1);
    assert(tPrime == .5);
    
    piece.calculateCurve(1, curveIndex, tPrime);
    assert(curveIndex == 1);
    assert(tPrime == 1);
    
    
    Vector3 piecewise_p_1 = piece.position(0);
    Vector3 circle_p_1 = circle.position(0);
    assert(piecewise_p_1 == circle_p_1);
    
    
    Vector3 piecewise_p_2 = piece.position(1);
    Vector3 circle_p_2 = square.position(1);
    assert(piecewise_p_2 == circle_p_2);
    
    
    
    
    
}

