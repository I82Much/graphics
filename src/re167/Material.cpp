#include "Material.h"
#include "Texture.h"
#include "Shader.h"

using namespace RE167;

const Material Material::OPEN_GL_DEFAULT;

Material::Material() :  mDiffuse(Vector3(0.8,0.8,0.8)),
                        mSpecular(Vector3(0,0,0)),
                        mAmbient(Vector3(0.2,0.2,0.2)), 
                        mShininess(0), mTexture(0), mShader(0)
{
}

/**
* Given a float array in the format specified in Materials.h,
* create a material from it.
* Skip the 4th, 8th, 12 ones because those are the alphas and we ignore that
* parameter.  Furthermore, we skip the emissive light.
*/
Material::Material(float m[13]) :   
                                    mDiffuse(Vector3(m[4], m[5], m[6])),
                                    mSpecular(Vector3(m[8], m[9], m[10])),
                                    mAmbient(Vector3(m[0], m[1], m[2])),
                                    mShininess(m[12]),
                                    mTexture(NULL),
                                    mShader(NULL)
{}

Material::Material(const Vector3 &diffuse, const Vector3 &specular,
                    const Vector3 &ambient, const float shininess,
                    Texture *texture, Shader *shader) :
                    mDiffuse(diffuse),
                    mSpecular(specular),
                    mAmbient(ambient),
                    mShininess(shininess),
                    mTexture(texture),
                    mShader(shader) {}
                    



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