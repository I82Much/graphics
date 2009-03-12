#ifndef __PGMReader_h__
#define __PGMReader_h__

#include "RE167_global.h"
#include <string>

namespace RE167 {

	class RE167_EXPORT PGMReader
	{
	public:
		static unsigned char* loadPGM(std::string filename, int& width, int& height);
	};
}

#endif 



