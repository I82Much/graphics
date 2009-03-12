#ifndef __ObjReader_h__
#define __ObjReader_h__

#include "RE167_global.h"

namespace RE167 {

	class RE167_EXPORT ObjReader
	{
	public:
		static void readObj(char* fileName, int &nVertices, float **vertices, float **normals, 
							 float **texcoords, int &nIndices, int **indices);

		static void normalize(float *vertices, int numVertices);

	private:
		static void get_indices(char *word, int *vindex, int *tindex, int *nindex);
	};



}

#endif // CSE167_OBJFILE_H_

