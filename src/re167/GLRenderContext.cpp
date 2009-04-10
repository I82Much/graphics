#include "GLRenderContext.h"
#include "Light.h"
#include "VertexDeclaration.h"
#include "Material.h"
#include "Texture.h"
#include "Shader.h"
#include "VertexData.h"

using namespace RE167;

GLRenderContext* GLRenderContext::getSingletonPtr(void)
{
	return static_cast<GLRenderContext *>(ms_Singleton);
}
GLRenderContext& GLRenderContext::getSingleton(void)
{  
	assert( ms_Singleton );  
	return ( *(static_cast<GLRenderContext *>(ms_Singleton)) );  
}

void GLRenderContext::init()
{
	assert(glewInit() == GLEW_OK);
	assert(GL_VERSION_2_0);
	
	wireframe = false;
    // Ensures that normal vectores will be scaled to unit length
    glEnable(GL_NORMALIZE);
    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLRenderContext::toggleWireframe() {
	wireframe = !wireframe;
	if (wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	endFrame();
}

void GLRenderContext::setViewport(int width, int height)
{
	glViewport(0,0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLRenderContext::beginFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLRenderContext::endFrame()
{
	glFlush();
}

void GLRenderContext::setModelViewMatrix(const Matrix4 &m)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(m.transpose().getElementPointer());
}

void GLRenderContext::setProjectionMatrix(const Matrix4 &m)
{
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(m.transpose().getElementPointer());
}

void GLRenderContext::render(Object *object)
{
//    setMaterial(object->getMaterial());
    
	VertexData& vertexData = object->vertexData;
	VertexDeclaration& vertexDeclaration = vertexData.vertexDeclaration;
	VertexBufferBinding& vertexBufferBinding = vertexData.vertexBufferBinding;

	// The basic way to draw triangles in OpenGL is to use glBegin(GL_TRIANGLES),
	// then specify vertices, colors, etc. using glVertex(), glColor(), etc., and finish
	// with glEnd(). However, we are using a more advanced technique here that is based
	// on so-called vertex arrays. Vertex arrays are more efficient because they need
	// fewer function calls to OpenGL. Read more about vertex arrays in Chapter 2 of the
	// OpenGL book.

	// Set up vertex arrays
	for(int j=0; j<vertexDeclaration.getElementCount(); j++)
	{
		const VertexElement *element = vertexDeclaration.getElement(j);

		const VertexBuffer& vertexBuffer = vertexBufferBinding.getBuffer(element->getBufferIndex());
		unsigned char* buf = vertexBuffer.getBuffer();

		GLsizei vertexStride = static_cast<GLsizei>(element->getStride());
		GLint vertexSize = static_cast<GLint>(element->getSize());
		int offset = element->getOffset();

		switch(element->getSemantic())
		{
			case VES_POSITION :
				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(vertexSize, GL_FLOAT, vertexStride, buf+offset);
				break;
			case VES_NORMAL :
				glEnableClientState(GL_NORMAL_ARRAY);
				glNormalPointer(GL_FLOAT, vertexStride, buf+offset);
				break;
			case VES_DIFFUSE :
				glEnableClientState(GL_COLOR_ARRAY);
				glColorPointer(vertexSize, GL_FLOAT, vertexStride, buf+offset);
				glEnable(GL_COLOR_MATERIAL);
				break;
			case VES_TEXTURE_COORDINATES :
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(vertexSize, GL_FLOAT, vertexStride, buf+offset);
				break;
		}
	}
	
	// Draw
	glDrawElements(GL_TRIANGLES, vertexData.getIndexCount(), GL_UNSIGNED_INT, vertexData.getIndexBuffer());

	// Disable the arrays we used
	for(int j=0; j<vertexDeclaration.getElementCount(); j++)
	{
		const VertexElement *element = vertexDeclaration.getElement(j);

		switch(element->getSemantic())
		{
			case VES_POSITION :
				glDisableClientState(GL_VERTEX_ARRAY);
				break;
			case VES_NORMAL :
				glDisableClientState(GL_NORMAL_ARRAY);
				break;
			case VES_DIFFUSE :
				glDisableClientState(GL_COLOR_ARRAY);
				break;
			case VES_TEXTURE_COORDINATES :
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				break;
		}
	}

	assert(glGetError()==GL_NO_ERROR);
}

// Add the following to GLRenderContext.cpp

void GLRenderContext::setLights(const std::list<Light*> &lightList)
{	
	GLint lightIndex[] = {GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, GL_LIGHT3, GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7};

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	std::list<Light*>::const_iterator iter;

	if(lightList.begin()!=lightList.end())
	{
		// Lighting
		glEnable(GL_LIGHTING);

		int i=0;
		for (iter=lightList.begin(); iter!=lightList.end() && i<8; iter++)
		{
			Light *l = (*iter);

			glEnable(lightIndex[i]);

			if(l->getType() == Light::DIRECTIONAL)
			{
				float direction[4];
				direction[0] = l->getDirection().getX();
				direction[1] = l->getDirection().getY();
				direction[2] = l->getDirection().getZ();
				direction[3] = 0.f;
				glLightfv(lightIndex[i], GL_POSITION, direction);
			}
			if(l->getType() == Light::POINT || l->getType() == Light::SPOT)
			{
				float position[4];
				position[0] = l->getPosition().getX();
				position[1] = l->getPosition().getY();
				position[2] = l->getPosition().getZ();
				position[3] = 1.f;
				glLightfv(lightIndex[i], GL_POSITION, position);
			}
			if(l->getType() == Light::SPOT)
			{
				float spotDirection[3];
				spotDirection[0] = l->getSpotDirection().getX();
				spotDirection[1] = l->getSpotDirection().getY();
				spotDirection[2] = l->getSpotDirection().getZ();
				glLightfv(lightIndex[i], GL_SPOT_DIRECTION, spotDirection);
				glLightf(lightIndex[i], GL_SPOT_EXPONENT, l->getSpotExponent());
				glLightf(lightIndex[i], GL_SPOT_CUTOFF, l->getSpotCutoff());
			}

			float diffuse[4];
			diffuse[0] = l->getDiffuseColor().getX();
			diffuse[1] = l->getDiffuseColor().getY();
			diffuse[2] = l->getDiffuseColor().getZ();
			diffuse[3] = 1.f;
			glLightfv(lightIndex[i], GL_DIFFUSE, diffuse);

			float ambient[4];
			ambient[0] = l->getAmbientColor().getX();
			ambient[1] = l->getAmbientColor().getY();
			ambient[2] = l->getAmbientColor().getZ();
			ambient[3] = 0;
			glLightfv(lightIndex[i], GL_AMBIENT, ambient);

			float specular[4];
			specular[0] = l->getSpecularColor().getX();
			specular[1] = l->getSpecularColor().getY();
			specular[2] = l->getSpecularColor().getZ();
			specular[3] = 0;
			glLightfv(lightIndex[i], GL_SPECULAR, specular);
			
			i++;
            
		}
	}
}



void GLRenderContext::setMaterial(const Material *m)
{
    // If there's no material, we need to ensure that
    // material, texture, and shaders are all at default
    // Do this by passing in the Default material
    if (m==NULL) {
        setMaterial(&Material::OPEN_GL_DEFAULT);
    }
    else 
	{
	    // Just in case they had used regular coloring previously.
        glDisable(GL_COLOR_MATERIAL);
	    
		float diffuse[4];
		diffuse[0] = m->getDiffuse().getX();
		diffuse[1] = m->getDiffuse().getY();
		diffuse[2] = m->getDiffuse().getZ();
		diffuse[3] = 1.f;
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);

		float ambient[4];
		ambient[0] = m->getAmbient().getX();
		ambient[1] = m->getAmbient().getY();
		ambient[2] = m->getAmbient().getZ();
		ambient[3] = 1.f;
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);

		float specular[4];
		specular[0] = m->getSpecular().getX();
		specular[1] = m->getSpecular().getY();
		specular[2] = m->getSpecular().getZ();
		specular[3] = 1.f;
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);

		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m->getShininess());




		Texture *tex = m->getTexture();
		if(tex!=0)
		{
			glEnable(GL_TEXTURE_2D);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glBindTexture(GL_TEXTURE_2D, tex->getId());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
        // No texture
		else {
            glDisable(GL_TEXTURE_2D);
		}

        // If the material does not include a texture, use the default OpenGL
        // shader
        if (m->getShader() == NULL) 
        {
            Shader::restoreDefaultShader();
        }
        else 
		{
			m->getShader()->use();
		}
	}
}
