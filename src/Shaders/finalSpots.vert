// Basic vertex shader for 2D texturing.

varying vec3 lightDir0, reflection0; // spot light
varying vec3 lightDir1, reflection1; // spot light

void setUpSpots(vec4 vertCoords, vec3 normal) {	

	// calculate the lightDir and reflection for gl_LightSource[0]
	// this is a spot light
	lightDir0 = normalize(vec3(gl_LightSource[0].position - vertCoords));
	reflection0 = normalize(-lightDir0 - (2.0 * max(0.0, dot(normal, -lightDir0)) * normal));

	// calculate the lightDir and reflection for gl_LightSource[1]
	// this is a spot light
	lightDir1 = normalize(vec3(gl_LightSource[1].position - vertCoords));
	reflection1 = -normalize(lightDir1 - (2.0 * max(0.0, dot(normal, lightDir1)) * normal));

}
