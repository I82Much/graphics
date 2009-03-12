#ifndef __Matrix4__
#define __Matrix4__

#include "RE167_global.h"
#include "Vector4.h"
#include <iostream>
#include <fstream>

namespace RE167
{
	/** Matrix class for 4x4 matrices.*/

    class RE167_EXPORT Matrix4
    {
		

    protected:
        union {
            float m[4][4];
            float _m[16];
        };

    public:
		static const Matrix4 IDENTITY;

        inline Matrix4()
        {
        }

		//friend ostream& operator<<(ostream &os,const Matrix4 &obj);

		//	friend ostream &operator<<(ostream &stream, Matrix4 ob);

		// Equality operator
		const bool operator==(const Matrix4 &other) const;
			
		// Inequality operator
		const bool operator!=(const Matrix4 &other) const;

        inline Matrix4(
            float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33 )
        {
            m[0][0] = m00;
            m[0][1] = m01;
            m[0][2] = m02;
            m[0][3] = m03;
            m[1][0] = m10;
            m[1][1] = m11;
            m[1][2] = m12;
            m[1][3] = m13;
            m[2][0] = m20;
            m[2][1] = m21;
            m[2][2] = m22;
            m[2][3] = m23;
            m[3][0] = m30;
            m[3][1] = m31;
            m[3][2] = m32;
            m[3][3] = m33;
        }
		
		/**
		* Given four column vectors, create
		* the corresponding matrix
		*/
		inline Matrix4(	const Vector4 &col1, 
						const Vector4 &col2, 
						const Vector4 &col3,
						const Vector4 &col4 )
		{
			m[0][0] = col1.getX();
			m[1][0] = col1.getY();
			m[2][0] = col1.getZ();
			m[3][0] = col1.getW();
			
			m[0][1] = col2.getX();
			m[1][1] = col2.getY();
			m[2][1] = col2.getZ();
			m[3][1] = col2.getW();
			
			m[0][2] = col3.getX();
			m[1][2] = col3.getY();
			m[2][2] = col3.getZ();
			m[3][2] = col3.getW();
			
			m[0][3] = col4.getX();
			m[1][3] = col4.getY();
			m[2][3] = col4.getZ();
			m[3][3] = col4.getW();
		}

		inline Matrix4 operator*(const Matrix4 &m2) const
		{
			Matrix4 r;
			r.m[0][0] = m[0][0] * m2.m[0][0] + m[0][1] * m2.m[1][0] + m[0][2] * m2.m[2][0] + m[0][3] * m2.m[3][0];
			r.m[0][1] = m[0][0] * m2.m[0][1] + m[0][1] * m2.m[1][1] + m[0][2] * m2.m[2][1] + m[0][3] * m2.m[3][1];
			r.m[0][2] = m[0][0] * m2.m[0][2] + m[0][1] * m2.m[1][2] + m[0][2] * m2.m[2][2] + m[0][3] * m2.m[3][2];
			r.m[0][3] = m[0][0] * m2.m[0][3] + m[0][1] * m2.m[1][3] + m[0][2] * m2.m[2][3] + m[0][3] * m2.m[3][3];

			r.m[1][0] = m[1][0] * m2.m[0][0] + m[1][1] * m2.m[1][0] + m[1][2] * m2.m[2][0] + m[1][3] * m2.m[3][0];
			r.m[1][1] = m[1][0] * m2.m[0][1] + m[1][1] * m2.m[1][1] + m[1][2] * m2.m[2][1] + m[1][3] * m2.m[3][1];
			r.m[1][2] = m[1][0] * m2.m[0][2] + m[1][1] * m2.m[1][2] + m[1][2] * m2.m[2][2] + m[1][3] * m2.m[3][2];
			r.m[1][3] = m[1][0] * m2.m[0][3] + m[1][1] * m2.m[1][3] + m[1][2] * m2.m[2][3] + m[1][3] * m2.m[3][3];

			r.m[2][0] = m[2][0] * m2.m[0][0] + m[2][1] * m2.m[1][0] + m[2][2] * m2.m[2][0] + m[2][3] * m2.m[3][0];
			r.m[2][1] = m[2][0] * m2.m[0][1] + m[2][1] * m2.m[1][1] + m[2][2] * m2.m[2][1] + m[2][3] * m2.m[3][1];
			r.m[2][2] = m[2][0] * m2.m[0][2] + m[2][1] * m2.m[1][2] + m[2][2] * m2.m[2][2] + m[2][3] * m2.m[3][2];
			r.m[2][3] = m[2][0] * m2.m[0][3] + m[2][1] * m2.m[1][3] + m[2][2] * m2.m[2][3] + m[2][3] * m2.m[3][3];

			r.m[3][0] = m[3][0] * m2.m[0][0] + m[3][1] * m2.m[1][0] + m[3][2] * m2.m[2][0] + m[3][3] * m2.m[3][0];
			r.m[3][1] = m[3][0] * m2.m[0][1] + m[3][1] * m2.m[1][1] + m[3][2] * m2.m[2][1] + m[3][3] * m2.m[3][1];
			r.m[3][2] = m[3][0] * m2.m[0][2] + m[3][1] * m2.m[1][2] + m[3][2] * m2.m[2][2] + m[3][3] * m2.m[3][2];
			r.m[3][3] = m[3][0] * m2.m[0][3] + m[3][1] * m2.m[1][3] + m[3][2] * m2.m[2][3] + m[3][3] * m2.m[3][3];

			return r;
        }


		static int runTestSuite();
		static int testTranslate();
		static int testScale();
		static int testRotateX();
		static int testRotateY();
		static int testRotateZ();
		static int testRotateArbitrary();




		// Matrix multiplied by vector
		Vector4 operator*(const Vector4 &vec) const;

		static Matrix4 scale(float xScale, float yScale, float zScale, 
							float wScale = 1.0f);

		
		static Matrix4 rotateX(float theta);
		static Matrix4 rotateY(float theta);
		static Matrix4 rotateZ(float theta);
		static Matrix4 rotate(const Vector4 &axis, float theta);
		
		static Matrix4 translate(float dx, float dy, float dz);

		inline Matrix4 transpose() const
		{
			Matrix4 r;
			for(int i=0; i<4; i++) 
			{
				for(int j=0; j<4; j++)
				{
					r._m[i*4+j]=_m[j*4+i];
				}
			}
			return r;
		}

		inline float* getElementPointer()
		{
			return static_cast<float *>(_m);
		}

		friend std::ostream &operator<<(std::ostream &os, const Matrix4 &obj);

	};

	inline std::ostream &operator<<(std::ostream &os, const Matrix4 &obj) { 
		for (int row = 0; row < 4; row++) {
			for (int col = 0; col < 4; col++) {
				os << obj.m[row][col] << "\t";
			}
			os << std::endl;
		}
		return os;
	}
	
}

#endif
