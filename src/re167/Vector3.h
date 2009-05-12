#ifndef __Vector3_h__
#define __Vector3_h__

#include "RE167_global.h"
#include <iostream>
#include "BasicMath.h"


namespace RE167 {
    class Vector4;
    
	class RE167_EXPORT Vector3 {

		public:
			// Default constructor; creates 0 vector
			Vector3(void);
			Vector3(float x, float y, float z);
			
            Vector3(const Vector4 &fourVec);
			
			~Vector3();

            static const Vector3 ZERO_VECTOR;

			static const float angleBetween(const Vector3 &a, const Vector3 &b);

			// Equality operator
			const bool operator==(const Vector3 &other) const;
			
			// Inequality operator
			const bool operator!=(const Vector3 &other) const;
			
			// Less than operator (for weak ordering/sorting)
            const bool operator<(const Vector3 &other) const;
			
			
			// Addition operator for vectors
			Vector3 & operator+=(const Vector3 &rhs);
			const Vector3 operator+(const Vector3 &rhs) const;

			// Subtraction operator for vectors
			Vector3 & operator-=(const Vector3 &rhs);
			const Vector3 operator-(const Vector3 &rhs) const;

			// Multiplication operator for scalar and vector
			Vector3 & operator*=(const float scalar);
					
			// Scalar multiplication
			friend Vector3 operator*(const Vector3 &vec, float scalar);
			friend Vector3 operator*(float scalar, const Vector3 &vec);	
			
			// Overload -Vector3  
			friend Vector3 operator-(const Vector3 &cCents);  



			// Dot product is a scalar operation
			const float dotProduct(const Vector3 &rhs) const;
			
			// Cross product in a vector operation
			const Vector3 crossProduct(const Vector3 &rhs) const;

			// Returns the normalized version of this vector.
			const Vector3 normalize() const;

			const float magnitude() const;
		
		const bool isUnitVector() const { return BasicMath::approxEqual(magnitude(), 1);}

			const float getX() const { return x; }
			const float getY() const { return y; }
			const float getZ() const { return z; }
		
			void setX(const float _x) { x = _x; }
			void setY(const float _y) { y = _y; }
			void setZ(const float _z) { z = _z; }
			
			static int runTestSuite();

			inline friend std::ostream &operator<<(std::ostream &os, const Vector3 &obj) {
				os << "(" << obj.x << ", " << obj.y << ", " << obj.z << ")";
				return os;
			}

		private:
			float x;
			float y;
			float z;
		};

			// Friend operator
		/* Scalar multiplication */
		inline Vector3 operator*(const Vector3 &vec, float scalar) {
			return Vector3( scalar * vec.x, scalar * vec.y, scalar * vec.z );
		}

		// Friend operator
		/* Scalar multiplication */
		inline Vector3 operator*(float scalar, const Vector3 &vec) {

			return Vector3( scalar * vec.x, scalar * vec.y, scalar * vec.z);
		}

		/* Unary negation.  Returns a vector pointing in opposite direction but with same magnitude */
		inline Vector3 operator-(const Vector3 &vec) {
			return Vector3( -vec.x, -vec.y, -vec.z );
		}
		
		
}

#endif
