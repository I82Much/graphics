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
	vec3 unitEyeDir = normalize(-position);

    // Local illumination - sum of 3 components.  diffuse, specular, ambient

    vec3 unitReflectionDir = normalize(reflectionDir);
    vec3 unitLightDir = normalize(lightDir);
    vec3 unitNormal = normalize(normal);
    
    vec4 color;
    color = vec4(0,0,0,0);
    
    // accumulate the colors for all the lights
    const int MAX_LIGHTS = 2;
    int i;
    for (i = 0; i < MAX_LIGHTS; i++) {
    
        // calculate the ambient color
        vec4 ambientColor = gl_LightSource[i].ambient * gl_FrontMaterial.ambient;
    
        // Need to take the max of 0 and the dot product because the dot product
        // can be negative
        float specCoefficient = pow(max(0.0, dot(unitEyeDir, unitReflectionDir)), gl_FrontMaterial.shininess);
    
        // Calculate the specular color
        vec4 specularColor = specCoefficient * gl_LightSource[i].specular * gl_FrontMaterial.specular;		
    
        float diffuseCoefficient = max(0.0, dot(unitLightDir, unitNormal));
        // Calculate the diffuse color
        vec4 diffuseColor = diffuseCoefficient * gl_LightSource[i].diffuse * gl_FrontMaterial.diffuse;
        
        color = color + ambientColor + specularColor + diffuseColor;
    }
    // Automatically clamped later    
    gl_FragColor = color; //ambientColor + specularColor + diffuseColor;
}
