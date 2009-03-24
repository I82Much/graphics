#include "Material.h"

using namespace RE167;

Material::Material() : mDiffuse(Vector3(0.8,0.8,0.8)), mSpecular(Vector3(0,0,0)),
 mAmbient(Vector3(0.2,0.2,0.2)), mShininess(0), mTexture(0), mShader(0)
{
}

void Material::setDiffuse(const Vector3 &diffuse)
{
	mDiffuse = diffuse;
}

const Vector3& Material::getDiffuse() const
{
	return mDiffuse;
}

void Material::setSpecular(const Vector3 &specular)
{
	mSpecular = specular;
}

const Vector3& Material::getSpecular() const
{
	return mSpecular;
}

void Material::setAmbient(const Vector3 &ambient)
{
	mAmbient = ambient;
}

const Vector3& Material::getAmbient() const
{
	return mAmbient;
}

void Material::setShininess(float shininess)
{
	mShininess = shininess;
}

float Material::getShininess() const
{
	return mShininess;
}

void Material::setTexture(Texture *texture)
{
	mTexture = texture;
}

Texture *Material::getTexture() const
{
	return mTexture;
}

void Material::setShader(Shader *shader)
{
	mShader = shader;
}

Shader *Material::getShader() const
{
	return mShader;
}