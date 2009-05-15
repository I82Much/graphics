// Basic fragment shader for 2D texture mapping.

// Define a variable to access the texture. 
uniform sampler2D tex;
uniform float eToThe500;

varying vec3 normal, eye;
varying vec3 lightDir0, reflection0;
varying vec3 lightDir1, reflection1;

void main() {		
	// The built-in function texture2D performs the texture
	// look-up. We read the texture coordinates from the
	// pre-defined varying variable gl_TexCoord[0], which
	// we set in the vertex shader.


	vec3 norm = normalize(normal);
	vec3 i = normalize(eye);

	vec4 diffuse0 = gl_LightSource[0].diffuse * max(dot(norm, normalize(lightDir0)),0.0) * gl_FrontMaterial.diffuse;
	vec4 specular0 = gl_LightSource[0].specular * max(pow(dot(normalize(reflection0), i), gl_FrontMaterial.shininess), 0.0) * gl_FrontMaterial.specular;



	vec4 ambient = gl_LightSource[0].ambient * gl_FrontMaterial.ambient;

	gl_FragColor = ambient;
	gl_FragColor += diffuse0 + specular0;


	// lightSource[1] is a spot light - commented out due to slow running time
	vec4 diffuseColor1 = vec4(0,0,0,0);
	vec4 specularColor1 = vec4(0,0,0,0);
	vec4 ambientColor1 = vec4(0,0,0,0);



	vec3 light1 = normalize(lightDir1);
	vec3 reflect1 = normalize(reflection1);
	vec3 spot1 = normalize(gl_LightSource[1].spotDirection);

	float normDotLightDir1 = max(dot(norm, light1),0.0);

	float spotEffect = dot(-light1, spot1);
	if (spotEffect > gl_LightSource[1].spotCosCutoff) {
		spotEffect = pow(spotEffect, gl_LightSource[1].spotExponent);
		diffuseColor1 = gl_LightSource[1].diffuse * spotEffect;
		specularColor1 = gl_LightSource[1].specular * spotEffect;
		ambientColor1 = gl_LightSource[1].ambient * spotEffect;
		
		vec4 diffuse1 = diffuseColor1 * normDotLightDir1 * gl_FrontMaterial.diffuse;
		vec4 specular1 = specularColor1 * pow(max(dot(reflect1, i), 0.0), gl_FrontMaterial.shininess) * gl_FrontMaterial.specular;
		
		gl_FragColor += diffuse1 + specular1 + ambientColor1 * gl_FrontMaterial.ambient;
		gl_FragColor *= texture2D(tex, gl_TexCoord[0].st);
	}
	else {
		gl_FragColor += gl_LightSource[1].ambient * gl_FrontMaterial.ambient;
		gl_FragColor *= texture2D(tex, gl_TexCoord[0].st);
	}

} 