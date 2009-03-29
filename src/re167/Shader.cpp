#include "Shader.h"
#include "glew.h"
#include <QtOpenGL>
#include <assert.h>

using namespace RE167;

Shader::Shader(char *vertexFileName, char *fragmentFileName)
{	
	char *vs = NULL,*fs = NULL;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

	vs = readSource(vertexFileName);
	fs = readSource(fragmentFileName);
	
	// If these assertions fail your shader couldn't be read from file.
	assert(fs!=NULL);
	assert(vs!=NULL);

	const char * ff = fs;
	const char * vv = vs;

	glShaderSource(v, 1, &vv,NULL);
	glShaderSource(f, 1, &ff,NULL);

	free(vs);free(fs);

	glCompileShader(v);
	glCompileShader(f);

	p = glCreateProgram();
	glAttachShader(p,f);
	glAttachShader(p,v);

	glLinkProgram(p);

	// If this assertion fails your shader programs couldn't be compiled
	// or linked by OpenGL.
	assert(glGetError()==GL_NO_ERROR);
}

void Shader::use()
{
	glUseProgram(p);

	// If this assertion fails your shader program couldn't be activated
	// by OpenGL.
	assert(glGetError()==GL_NO_ERROR);
}

void Shader::disable()
{
	glUseProgram(0);
}

void Shader::restoreDefaultShader() 
{
    glUseProgram(0);
}


char *Shader::readSource(char *fn) {

	FILE *fp;
	char *content = NULL;

	int count=0;

	if (fn != NULL) {
		fp = fopen(fn,"rt");

		if (fp != NULL) {
      
      fseek(fp, 0, SEEK_END);
      count = ftell(fp);
      rewind(fp);

			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count+1));
				count = fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}
	return content;
}