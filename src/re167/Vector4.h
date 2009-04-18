#ifndef __Vector4_h__
#define __Vector4_h__

#include "RE167_global.h"
#include <iostream>
#include "Vector3.h"

namespace RE167 {

	class RE167_EXPORT Vector4
	{
		public:
			// Default constructor; creates 0 vector
			Vector4(void);
			Vector4(float x, float y, float z, float w);

            Vector4(const Vector3 &val);
			~Vector4();

            static const Vector4 ZERO_VECTOR;
            static const Vector4 ORIGIN;

			static float angleBetween(const Vector4 &first, const Vector4 &second);

			static int runTestSuite();

			// Equality operator
			const bool operator==(const Vector4 &other) const;

			// Inequality operator
			const bool operator!=(const Vector4 &other) const;

			// Unary negation; returns vector pointing in opposite direction
			// but with same magnitude
			friend Vector4 operator-(const Vector4 &other);

            // Less than operator (for weak ordering/sorting)
            const bool operator<(const Vector4 &other) const;
            

			// Addition operator for vectors
			Vector4 & operator+=(const Vector4 &rhs);
			const Vector4 operator+(const Vector4 &rhs) const;



			// Subtraction operator for vectors
			Vector4 & operator-=(const Vector4 &rhs);
			const Vector4 operator-(const Vector4 &rhs) const;

			// Scalar multiplication
			friend Vector4 operator*(const Vector4 &vec, float scalar);
			friend Vector4 operator*(float scalar, const Vector4 &vec);

			// Multiplication operator for scalar and vector
			Vector4 & operator*=(const float scalar);

			// Dot product is a scalar operation
			const float dotProduct(const Vector4 &rhs) const;

			// Returns the normalized version of this vector.
			const Vector4 normalize();

			const float magnitude() const;

			const float getX() const { return x; }
			const float getY() const { return y; }
			const float getZ() const { return z; }
			const float getW() const { return w; }

			void setX(const float _x) { x = _x; }
			void setY(const float _y) { y = _y; }
			void setZ(const float _z) { z = _z; }
			void setW(const float _w) { w = _w; }

			inline friend std::ostream &operator<<(std::ostream &os, const Vector4 &obj) {
				os << "(" << obj.x << ", " << obj.y << ", " << obj.z << ", " << obj.w << ")";
				return os;
			}

	private:
		float x, y, z, w;
	};



	/* Friend operator:
	Scalar multiplication */
	inline Vector4 operator*(const Vector4 &vec, float scalar) {
		return Vector4( scalar * vec.x,
						scalar * vec.y,
						scalar * vec.z,
						scalar * vec.w );
	}

	/* Friend operator:
	* Scalar multiplication */
	inline Vector4 operator*(float scalar, const Vector4 &vec) {
		return Vector4( scalar * vec.x,
						scalar * vec.y,
						scalar * vec.z,
						scalar * vec.w );
	}

	/* Unary negation.  Returns a vector pointing in opposite direction but with same magnitude */
	inline Vector4 operator-(const Vector4 &other) {
		return Vector4( -other.x, -other.y, -other.z, -other.w );
	}

	/*
	inline std::ostream &operator<<(std::ostream &os, const Vector4 &obj) {
		os << "(" << obj.x << ", " << obj.y << ", " << obj.z << ", " << obj.w << ")";
		return os;
	}*/

}

#endif
