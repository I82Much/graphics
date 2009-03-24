#include "Light.h"

using namespace RE167;

Light::Light()
: mDirection(0,0,1), mPosition(0,0,1), mType(DIRECTIONAL), mDiffuse(1,1,1), mAmbient(0,0,0), mSpecular(1,1,1),
  mSpotDirection(0,0,1), mSpotExponent(0.f), mSpotCutoff(180.f)
{
}

void Light::setType(Light::Type type)
{
	mType = type;
}

Light::Type Light::getType() const
{
	return mType;
}

void Light::setDirection(const Vector3 &direction)
{
	mDirection = direction;
}

const Vector3 &Light::getDirection() const
{
	return mDirection;
}

void Light::setPosition(const Vector3 &position)
{
	mPosition = position;
}

const Vector3 &Light::getPosition() const
{
	return mPosition;
}

void Light::setDiffuseColor(const Vector3 &diffuse)
{
	mDiffuse = diffuse;
}

const Vector3 &Light::getDiffuseColor() const
{
	return mDiffuse;
}

void Light::setAmbientColor(const Vector3 &ambient)
{
	mAmbient = ambient;
}

const Vector3 &Light::getAmbientColor() const
{
	return mAmbient;
}

void Light::setSpecularColor(const Vector3 &specular)
{
	mSpecular = specular;
}

const Vector3 &Light::getSpecularColor() const
{
	return mSpecular;
}

void Light::setSpotDirection(const Vector3 &spotDirection)
{
	mSpotDirection = spotDirection;
}

const Vector3 &Light::getSpotDirection() const
{
	return mSpotDirection;
}

void Light::setSpotExponent(float spotExponent)
{
	mSpotExponent = spotExponent;
}

float Light::getSpotExponent() const
{
	return mSpotExponent;
}

void Light::setSpotCutoff(float spotCutoff)
{
	mSpotCutoff = spotCutoff;
}

float Light::getSpotCutoff() const
{
	return mSpotCutoff;
}