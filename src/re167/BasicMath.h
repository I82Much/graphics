#ifndef __BasicMath_h__
#define __BasicMath_h__

#include "RE167_global.h"


namespace RE167
{
    class Vector4;
    class Vector3;
    
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
		static float radians(float a) { return a*PI/180.f; };
		static float degrees(float radians) { return radians * 180.f / PI; }

		static bool AlmostEqual2sComplement(float A, float B, int maxUlps);

		static bool approxEqual (float A, float B);

		static float clamp(const float val, const float min, const float max);

		static float randBetween(const float min, const float max);

        static Vector4 lerp(const Vector4 &first, const Vector4 &second, float proportion);
        static Vector3 lerp(const Vector3 &first, const Vector3 &second, float proportion);

        static void split(float real, int &intPart, float &realPart);

	private:
		static const int MAX_NUM_DIGITS_DIFFERENCE = 1;
	};


}

#endif
