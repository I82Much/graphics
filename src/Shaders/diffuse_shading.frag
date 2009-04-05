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
    // TODO: Make this work with more than one light
    
	// We are in eye coordinates, so (0,0,0) - position is eye direction
	vec3 unitEyeDir = normalize(-position);

    // Local illumination - sum of 3 components.  diffuse, specular, ambient

    vec3 unitReflectionDir = normalize(reflectionDir);
    vec3 unitLightDir = normalize(lightDir);
    vec3 unitNormal = normalize(normal);
    
    // Handle just one light for now
    
    // calculate the ambient color
    vec4 ambientColor = gl_LightSource[0].ambient * gl_FrontMaterial.ambient;
    
    // Need to take the max of 0 and the dot product because the dot product
    // can be negative
    float specCoefficient = pow(max(0.0, dot(unitEyeDir, unitReflectionDir)), gl_FrontMaterial.shininess);
    
    // Calculate the specular color
    vec4 specularColor = specCoefficient * gl_LightSource[0].specular * gl_FrontMaterial.specular;		
    
    float diffuseCoefficient = max(0.0, dot(unitLightDir, unitNormal));
    // Calculate the diffuse color
    vec4 diffuseColor = diffuseCoefficient * gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse;
    
    // Automatically clamped later    
    gl_FragColor = ambientColor + specularColor + diffuseColor;
}
