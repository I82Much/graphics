// Basic fragment shader for 2D texture mapping.

// Define a variable to access the texture. 
uniform sampler2D tex;
uniform float eToThe500;

varying vec3 normal, eye; // 6 components

//vec4 colorFromPoints (vec3 norm, vec3 i);
vec4 colorFromSpots (vec3 norm, vec3 i);

void main() {		
	// The built-in function texture2D performs the texture
	// look-up. We read the texture coordinates from the
	// pre-defined varying variable gl_TexCoord[0], which
	// we set in the vertex shader.


	vec3 norm = normalize(normal);
	vec3 i = normalize(eye);

	vec4 colorFromSpotLights = colorFromSpots(norm, i);
//	vec4 colorFromPointLights = colorFromPoints(norm, i);

	gl_FragColor = colorFromSpotLights;
//	gl_FragColor += colorFromPointLights;
	gl_FragColor *= gl_TexCoord[0];

} 

