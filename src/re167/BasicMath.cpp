#include "BasicMath.h"
#include <math.h>
#include <assert.h>
#include <cstdlib> 
#include "Vector4.h"
#include "Vector3.h"


using namespace RE167;

const float BasicMath::PI = float(4.0 * atan( 1.0 ));
const float BasicMath::TWO_PI = 2.0 * PI;

bool BasicMath::approxEqual(float A, float B) {
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


Vector4 BasicMath::lerp(const Vector4 &first, const Vector4 &second, float proportion)
{
    assert(0 <= proportion && proportion <= 1.0);
    return ((1.0 - proportion) * first) + (proportion * second);
}

Vector3 BasicMath::lerp(const Vector3 &first, const Vector3 &second, float proportion)
{
    assert(0 <= proportion && proportion <= 1.0);
    return ((1.0 - proportion) * first) + (proportion * second);
}

