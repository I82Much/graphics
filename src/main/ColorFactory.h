#ifndef _COLOR_FACTORY_H__
#define _COLOR_FACTORY_H__

/**
* This class provides public methods for coloring objects.
*/
class ColorFactory 
{
public:

	static const int NUM_COMPONENTS_PER_COLOR;

	static void randomlyColorize(float *&colorArray, const int numElements);
    static void colorize(float *&colorArray, const int numElements);
	static void colorize(float *&colorArray, const int numElements, 
						const float r, const float g, const float b);
	static void colorize(float *&colorArray, const int numElements, const float *rgb);
	static void perturb(float *&colorArray, const int numElements, const float maxPerturbation = .1f);

	static void matchNormalizedVertices(float *&colorArray, float *&vertices, const int numElements);

	static void colorTerrain(float *&colorArray, float *vertices, const int numVertices);
};


#endif