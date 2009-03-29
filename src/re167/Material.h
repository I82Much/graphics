#ifndef __Material_h__
#define __Material_h__

#include "RE167_global.h"
#include "Vector3.h"
#include "Texture.h"
#include "Shader.h"

namespace RE167 {

	class RE167_EXPORT Material
	{
	public:
		Material();
        Material(float materialDefinition[13]);
        void setDiffuse(const Vector3 &diffuse);
		const Vector3& getDiffuse() const;
		void setSpecular(const Vector3 &specular);
		const Vector3& getSpecular() const;
		void setAmbient(const Vector3 &ambient);
		const Vector3& getAmbient() const;
		void setShininess(float shininess);
		float getShininess() const;
		void setTexture(Texture *texture);
		Texture *getTexture() const;
		void setShader(Shader *shader);
		Shader *getShader() const;

	private:
		Vector3 mDiffuse;
		Vector3 mSpecular;
		Vector3 mAmbient;
		float mShininess;
		Texture *mTexture;
		Shader *mShader;
	};
}

#endif