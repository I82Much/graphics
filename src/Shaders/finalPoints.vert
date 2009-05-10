// Basic vertex shader for 2D texturing.

varying vec3 lightDir2, reflection2; // point light
varying vec3 lightDir3, reflection3; // point light
//varying vec3 lightDir4, reflection4; // point light
//varying vec3 lightDir5, reflection5; // point light
//varying vec3 lightDir6, reflection6; // point light
//varying vec3 lightDir7, reflection7; // point light
varying float length2, length3;//, length4;//, length5, length6, length7;

void setUpPoints(vec4 vertCoords, vec3 normal) {	

	// calculate the lightDir and reflection for gl_LightSource[2]
	// this is a point light
	lightDir2 = normalize(vec3(gl_LightSource[2].position - vertCoords));
	reflection2 = -normalize(lightDir2 - (2.0 * max(0.0, dot(normal, lightDir2)) * normal));
	length2 = length(vec3(gl_LightSource[2].position - vertCoords));

	// calculate the lightDir and reflection for gl_LightSource[3]
	// this is a point light
	lightDir3 = normalize(vec3(gl_LightSource[3].position - vertCoords));
	reflection3 = -normalize(lightDir3 - (2.0 * max(0.0, dot(normal, lightDir3)) * normal));
	length3 = length(vec3(gl_LightSource[3].position - vertCoords));

	// calculate the lightDir and reflection for gl_LightSource[4]
	// this is a point light
//	lightDir4 = normalize(vec3(gl_LightSource[4].position - vertCoords));
//	reflection4 = -normalize(lightDir4 - (2.0 * max(0.0, dot(normal, lightDir4)) * normal));
//	length4 = length(vec3(gl_LightSource[4].position - vertCoords));

/*
	// calculate the lightDir and reflection for gl_LightSource[5]
	// this is a point light
	lightDir5 = normalize(vec3(gl_LightSource[5].position - vertCoords));
	reflection5 = -normalize(lightDir5 - (2.0 * max(0.0, dot(normal, lightDir5)) * normal));
	length5 = length(vec3(gl_LightSource[5].position - vertCoords));


	// calculate the lightDir and reflection for gl_LightSource[6]
	// this is a point light
	lightDir6 = normalize(vec3(gl_LightSource[6].position - vertCoords));
	reflection6 = -normalize(lightDir6 - (2.0 * max(0.0, dot(normal, lightDir6)) * normal));
	length6 = length(vec3(gl_LightSource[6].position - vertCoords));

	// calculate the lightDir and reflection for gl_LightSource[7]
	// this is a point light
	lightDir7 = normalize(vec3(gl_LightSource[7].position - vertCoords));
	reflection7 = -normalize(lightDir7 - (2.0 * max(0.0, dot(normal, lightDir7)) * normal));
	length7 = length(vec3(gl_LightSource[7].position - vertCoords));
*/
}
