#ifndef BEZIERCURVEREADER_H_842SROZB
#define BEZIERCURVEREADER_H_842SROZB

#include "RE167_global.h"

namespace RE167 {
    class BezierCurve;

	class RE167_EXPORT BezierCurveReader
	{
	public:
        static BezierCurve readCurve(char* fileName);
	};



}


#endif /* end of include guard: BEZIERCURVEREADER_H_842SROZB */
