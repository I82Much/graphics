// Basic vertex shader for 2D texturing.

varying vec3 normal, eye;

//void setUpPoints(vec4 vert, vec3 norm);
void setUpSpots(vec4 vert, vec3 norm);

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

	setUpSpots(vertCoords, normal);
//	setUpPoints(vertCoords, normal);

	// ftransform() is a built-in function that applies all
	// transformations (i.e., modelview and 
	// projection) to a vertex.

	gl_Position = ftransform();
}
