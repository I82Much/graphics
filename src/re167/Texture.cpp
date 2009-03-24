#include "Texture.h"
#include <glew.h>
#include <assert.h>

using namespace RE167;

Texture::Texture(QImage *tex)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, (GLuint*)&mId);
	glBindTexture(GL_TEXTURE_2D, mId);

	GLvoid *buf = (GLvoid*)getData(tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->width(), tex->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, buf);
	delete buf;

	// If this assertion fails your texture wasn't passed to OpenGL
	// successfully.
	assert(glGetError()==GL_NO_ERROR);
}

int Texture::getId() const
{
	return mId;
}

int *Texture::getData(QImage *tex)
{
	int *buf;
	buf = new int[tex->width()*tex->height()];
	for(int i=0; i<tex->height(); i++)
	{
		for(int j=0; j<tex->width(); j++)
		{
			buf[(tex->height()-i-1)*tex->width()+j]=qRed(tex->pixel(j,i)) | qGreen(tex->pixel(j,i)) << 8 | qBlue(tex->pixel(j,i)) << 16;
		}
	}
	return buf;
}