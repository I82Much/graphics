#ifndef __BasicMath_h__
#define __BasicMath_h__

#include "RE167_global.h"
#include <math.h>

namespace RE167
{
	/** Basic math utility class.
	@remarks
		Implements some basic math utilities and defines useful constants,
		such as PI.
	*/
	class RE167_EXPORT BasicMath
	{
	public:
		static const float PI;
		static const float TWO_PI;
		static float radian(float a) { return a*PI/180.f; };
		static float degrees(float radians) { return radians * 180.f / PI; }

		static bool AlmostEqual2sComplement(float A, float B, int maxUlps);

		static bool approxEqual (float A, float B);

		static float clamp(const float val, const float min, const float max);

		static float randBetween(const float min, const float max);

	private:
		static const int MAX_NUM_DIGITS_DIFFERENCE = 1;
	};


}

#endif
