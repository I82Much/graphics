#ifndef __Light_h__
#define __Light_h__

#include "RE167_global.h"
#include "Vector3.h"

namespace RE167 {

	class RE167_EXPORT Light
	{
	public:

		enum Type {
			DIRECTIONAL = 1,
			POINT = 2,
			SPOT = 3,
		};

		Light();

		void setType(const Type type);
		Type getType() const;
		void setDirection(const Vector3 &direction);
		const Vector3 &getDirection() const;
		void setPosition(const Vector3 &position);
		const Vector3 &getPosition() const;
		void setDiffuseColor(const Vector3 &diffuse);
		const Vector3 &getDiffuseColor() const;
		void setSpecularColor(const Vector3 &specular);
		const Vector3 &getSpecularColor() const;
		void setAmbientColor(const Vector3 &ambient);
		const Vector3 &getAmbientColor() const;
		void setSpotDirection(const Vector3 &spotDirection);
		const Vector3 &getSpotDirection() const;
		void setSpotExponent(float spotExponent);
		float getSpotExponent() const;
		void setSpotCutoff(float spotCutoff);
		float getSpotCutoff() const;

	private:
		Vector3 mDirection;
		Vector3 mPosition;
		Vector3 mDiffuse;
		Vector3 mSpecular;
		Vector3 mAmbient;
		Vector3 mSpotDirection;
		float mSpotExponent;
		float mSpotCutoff;
		Type mType;
	};


}

#endif