#ifndef __Shader_h__
#define __Shader_h__

#include "RE167_global.h"

namespace RE167 {

	class RE167_EXPORT Shader
	{
	public:
		/** Load a vertex and fragment shader from file.
		*/
		Shader(char *vertexFileName, char *fragmentFileName);
		Shader(char** vertexFileNames, char** fragmentFileNames, int length);

		/** Activate the shader.
		*/
		void use();

		/** Disable the shader.
		*/
		void disable();
		
        static void restoreDefaultShader();

	protected:
		char* readSource(char *fn);
		unsigned int v,f,p;
	};

}

#endif