// Vertex shader for per-pixel diffuse shading.The
// teapot is a good object to test this shader, since
// the .obj file includes surface normals.

// The shader computes the unit light direction and unit
// surface normal, which are passed to the fragment
// shader as varying variables.

varying vec3 normal, lightDir, reflectionDir, position;
varying vec4 color;

void main()
{	
	// Note that gl_LightSource, gl_NormalMatrix, and gl_Normal
	// are pre-defined variables that access the current OpenGL
	// state.
	lightDir = normalize(vec3(gl_LightSource[0].position));
	normal = normalize(gl_NormalMatrix * gl_Normal);
	
	
	// Calculate the reflectance vector, which is defined such that
	// the angle between lightDir and the normal is the same as the angle 
	// between the normal and the reflectance vector; in other words the vector
	// lightDir reflected about normal.
	// Equation is given in Fundamentals of Computer Graphics Second Edition,
	// p. 196
	
	float dotProd = dot(lightDir, normal);
			
	reflectionDir = -lightDir + ((2.0 * dotProd) * normal );

	position = vec3(gl_ModelViewMatrix * gl_Vertex);
	
	
	// ftransform() is a built-in function that applies all
	// transformations (i.e., modelview and 
	// projection) to a vertex.
	gl_Position = ftransform();
}
