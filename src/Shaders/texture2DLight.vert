// Basic vertex shader for 2D texturing.

varying vec3 normal, eye;
varying vec3 lightDir0, reflection0; // directional light
varying vec3 lightDir1, reflection1; // spot light

void main() {	
	// gl_MultiTexCoord is a pre-defined vertex attribute that
	// stores the texture coordinates of the vertex. gl_TexCoord[0]
	// is a pre-defined varying variable that is passed to the 
	// fragment shader.	
	gl_TexCoord[0] = gl_MultiTexCoord0;

	// Note that gl_LightSource, gl_NormalMatrix, and gl_Normal
	// are pre-defined variables that access the current OpenGL
	// state.

	vec4 vertCoords = gl_ModelViewMatrix * gl_Vertex;
	
	// calculate normal and eye vector for the fragment
	normal = normalize(gl_NormalMatrix * gl_Normal);
	eye = -normalize(vec3(vertCoords));

	// calculate the lightDir and reflection for gl_LightSource[0]
	// this is a directional light
	lightDir0 = normalize(vec3(gl_LightSource[0].position));
	reflection0 = normalize(-lightDir0 - (2.0 * max(0.0, dot(normal, -lightDir0)) * normal));

	// calculate the lightDir and reflection and color for gl_LightSource[1]
	// this is a spot light
	lightDir1 = normalize(vec3(gl_LightSource[1].position - vertCoords));
	reflection1 = -normalize(lightDir1 - (2.0 * max(0.0, dot(normal, lightDir1)) * normal));

	// ftransform() is a built-in function that applies all
	// transformations (i.e., modelview and 
	// projection) to a vertex.

	gl_Position = ftransform();
}
