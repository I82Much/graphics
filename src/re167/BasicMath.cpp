#include "BasicMath.h"
#include <assert.h>
#include <cstdlib> 

using namespace RE167;

const float BasicMath::PI = float(4.0 * atan( 1.0 ));
const float BasicMath::TWO_PI = 2.0 * PI;

//const int BasicMath::MAX_NUM_DIGITS_DIFFERENCE = 1;


/**
* @author Bruce Dawson
* Usable AlmostEqual function
* Taken from http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm
* @return true iff A and B are within maxUlps different in the least significant place
*/ 
bool BasicMath::AlmostEqual2sComplement(float A, float B, int maxUlps) 
{
    // Make sure maxUlps is non-negative and small enough that the
    // default NAN won't compare as equal to anything.
    assert(maxUlps > 0 && maxUlps < 4 * 1024 * 1024);
    int aInt = *(int*)&A;
    // Make aInt lexicographically ordered as a twos-complement int
    if (aInt < 0)
        aInt = 0x80000000 - aInt;
    // Make bInt lexicographically ordered as a twos-complement int
    int bInt = *(int*)&B;
    if (bInt < 0)
        bInt = 0x80000000 - bInt;
    int intDiff = abs(aInt - bInt);
    if (intDiff <= maxUlps)
        return true;
    return false;
}


bool BasicMath::approxEqual(float A, float B) {
	//return AlmostEqual2sComplement(A, B, 4);//MAX_NUM_DIGITS_DIFFERENCE);

	return fabs(A - B) < .001;
}

/**
* Clamps the value of val to be between min and max
*/
float BasicMath::clamp(const float val, const float min, const float max) {
	if (val < min) {
		return min;
	}
	else if (val > max) {
		return max;
	}
	else {
		return val;
	}
}

// Based off of code from http://www.codeguru.com/forum/showthread.php?t=351834
// @author flaviorodriguez
float BasicMath::randBetween(const float min, const float max) {
	return ((max-min)*((float)rand()/RAND_MAX))+min;
}

