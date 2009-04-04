// Fragment shader for per-pixel diffuse shading. The
// teapot is a good object to test this shader, since
// the .obj file includes surface normals.

// The shader computes the dot product between the unit
// surface normal and light direction, which were 
// passed as varying inputs from the vertex shader. The
// result is multiplied with the vertex color, which is 
// accessed through a pre-defined varying variable.

varying vec3 normal, lightDir, reflectionDir, position;

void main()
{		
	// We are in eye coordinates, so (0,0,0) - position is eye direction
	vec3 eyeDir = normalize(-position);

	float phongExponent = 100.0;


    // Local illumination - sum of 3 components.  diffuse, specular, ambient
    // Complete blinn model:
    // c = sum of cl_i (k_d (L_i dot n) + K_s (h))


	//gl_FragColor = gl_LightSource[0].diffuse * pow(max(dot(normal, normalize(lightDir)),0.0), phongExponent) * gl_FrontMaterial.diffuse;	
	gl_FragColor = gl_LightSource[0].diffuse * max(0.0, dot(normalize(eyeDir), normalize(reflectionDir) ) ) * 		
					gl_FrontMaterial.diffuse;		
					
					
					
}
