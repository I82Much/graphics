#include "ColorFactory.h"
#include <cstdlib>           // for random numbers
#include <iostream>
#include "BasicMath.h"

/**
* This class consists of static methods that perform
* coloring tasks to geometry in a scene.
* @author Nicholas Dunn
* @date   February 21, 2009
*/


const int ColorFactory::NUM_COMPONENTS_PER_COLOR = 3;

void ColorFactory::randomlyColorize(float *&colorArray, const int numElements) {
    for (int i = 0; i < numElements; i++) {
        colorArray[i] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    }
}

/**
* Given an array of colors and an array of vertices that lie within the unit cube,
* make the rgb values match the abs(x), abs(y), abs(z) components.
*/
void ColorFactory::matchNormalizedVertices(float *&colorArray, float *&vertices, const int numElements) {
	for (int i = 0; i < numElements; i++) {
		colorArray[i] = abs(vertices[i]);
	}
}

/**
* Given an array of colors and an array of vertices that lie within the unit cube,
* make the rgb values depend on the height of the points.
*/
void ColorFactory::colorTerrain(float *&colorArray, float *vertices, const int numVertices) {
	const float SEA_LEVEL = -.50f;
	const float SAND_LEVEL = 0.0f;
	const float GRASS_LEVEL = .30f;
	const float DIRT_LEVEL = .60f;

	for (int i = 0; i < numVertices; i++) {
		int offset = i * 3;

		// Y value
		float height = vertices[offset + 1];
		float r, g, b;

		// sea = blue
		if (height <= SEA_LEVEL) {
			r = 0;
			g = 0;
			b = 1;
		}
		// Sand = yellow
		else if (height <= SAND_LEVEL) {
			r = 245.0f/256.0f;
			g = 184.0f/256.0f;
			b = 0;
		}
		// grass = green
		else if (height <= GRASS_LEVEL) {
			r = 0;
			g = 1;
			b = 0;
		}
		// dirt = brown
		else if (height <= DIRT_LEVEL) {
			r = 184.0f / 256.0f;
			g = 92.0f / 256.0f;
			b = 0;
		}
		// snow = white
		else {
			r = 1;
			g = 1;
			b = 1;
		}

		colorArray[offset] = r;
		colorArray[offset + 1] = g;
		colorArray[offset + 2] = b;

	}
}


// Fills the color array with alternating red, green, blue pattern
void ColorFactory::colorize(float *&colorArray, const int numElements) {
	const int numFaces = numElements/9;

	// The faces go in order red, green, blue
	for (int i = 0; i < numFaces; i++) {
		float r, g, b;

		switch (i % 3) {
			// The first, fourth... will be red
			case 0:
				r = 1.0f;
				g = 0.0f;
				b = 0.0f;
				break;
			// The second, fifth... will be green
			case 1:
				r = 0.0f;
				g = 1.0f;
				b = 0.0f;
				break;
			// The third, sixth... will be blue
			case 2:
				r = 0.0f;
				g = 0.0f;
				b = 1.0f;
				break;
		}
		colorArray[(i * 9)] = r;
		colorArray[(i * 9) + 1] = g;
		colorArray[(i * 9) + 2] = b;

		colorArray[(i * 9) + 3] = r;
		colorArray[(i * 9) + 4] = g;
		colorArray[(i * 9) + 5] = b;
			
		colorArray[(i * 9) + 6] = r;
		colorArray[(i * 9) + 7] = g;
		colorArray[(i * 9) + 8] = b;
	}
}
	

// Color the entire surface the color of rgb
void ColorFactory::colorize(float *&colorArray, 
							   const int numElements, 
							   const float *rgb) {
	ColorFactory::colorize(colorArray, numElements, rgb[0], rgb[1], rgb[2]);
}



// Fills the color array with the given shade of color
void ColorFactory::colorize(float *&colorArray, 
								const int numElements, 
								const float r, 
								const float g, 
								const float b) {
	for (int i = 0; i < numElements; i++) {
		switch (i % NUM_COMPONENTS_PER_COLOR) {
			case 0:
				colorArray[i] = r;
				break;
			case 1:
				colorArray[i] = g;
				break;
			case 2:
				colorArray[i] = b;
				break;
			default:
				std::cerr << "Error, unexpected value in GeometryFactory::colorize" << std::endl;
		}
	}
}

/**
* Given an array of colors, perturbs each element
* by a random amount, clamping the rgb values to be
* legal values
*/
void ColorFactory::perturb(float *&colorArray, 
						   const int numElements, 
						   float maxPerturbation) {
	for (int i = 0; i < numElements; i++) {
		float perturbation = RE167::BasicMath::randBetween(-maxPerturbation, maxPerturbation);
		colorArray[i] = RE167::BasicMath::clamp(colorArray[i] + perturbation, 0.0f, 1.0f);
	}
}
