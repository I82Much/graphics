#include "GLRenderContext.h"

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
    std::cout<<"Initing"<< std::endl;
    
	assert(glewInit() == GLEW_OK);
	assert(GL_VERSION_2_0);
	
	wireframe = false;

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
