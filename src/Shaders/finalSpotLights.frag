// Basic fragment shader for 2D texture mapping.

uniform sampler2D tex;

varying vec3 normal, eye;
varying vec3 lightDir0, reflection0; // 6 components
varying vec3 lightDir1, reflection1; // 6 components


void main() {		
	// The built-in function texture2D performs the texture
	// look-up. We read the texture coordinates from the
	// pre-defined varying variable gl_TexCoord[0], which
	// we set in the vertex shader.

	vec3 norm = normalize(normal);
	vec3 i = normalize(eye);

	// gl_LightSource[0] is a spot light
	vec4 diffuseColor0 = vec4(0,0,0,0);
	vec4 specularColor0 = vec4(0,0,0,0);
	vec4 ambientColor0 = vec4(0,0,0,0);
	vec4 colorFrom0 = vec4(0.0,0.0,0.0,1.0);

	vec3 light0 = normalize(lightDir0);
	vec3 reflect0 = normalize(reflection0);
	vec3 spot0 = normalize(gl_LightSource[0].spotDirection);

	float normDotLightDir0 = max(dot(norm, light0),0.0);

	float spotEffect0 = dot(-light0, spot0);
	if (spotEffect0 > gl_LightSource[0].spotCosCutoff) {
		spotEffect0 = pow(spotEffect0, gl_LightSource[0].spotExponent);
		diffuseColor0 = gl_LightSource[0].diffuse * spotEffect0;
		specularColor0 = gl_LightSource[0].specular * spotEffect0;
		ambientColor0 = gl_LightSource[0].ambient * spotEffect0;
		
		vec4 diffuse0 = diffuseColor0 * normDotLightDir0 * gl_FrontMaterial.diffuse;
		vec4 specular0 = specularColor0 * pow(max(dot(reflect0, i), 0.0), gl_FrontMaterial.shininess) * gl_FrontMaterial.specular;
		
		colorFrom0 += diffuse0 + specular0 + ambientColor0 * gl_FrontMaterial.ambient;

	gl_FragColor = colorFrom0;// + colorFrom1;
	gl_FragColor *= gl_TexCoord[0];

	}
	else {
		colorFrom0 += gl_LightSource[0].ambient * gl_FrontMaterial.ambient;

	gl_FragColor = colorFrom0;// + colorFrom1;
	gl_FragColor *= gl_TexCoord[0];

	}


	// gl_LightSource[1] is a spot light
	vec4 diffuseColor1 = vec4(0,0,0,0);
	vec4 specularColor1 = vec4(0,0,0,0);
	vec4 ambientColor1 = vec4(0,0,0,0);
	vec4 colorFrom1 = vec4(0.0,0.0,0.0,0.0);

	vec3 light1 = normalize(lightDir1);
	vec3 reflect1 = normalize(reflection1);
	vec3 spot1 = normalize(gl_LightSource[1].spotDirection);

	float normDotLightDir1 = max(dot(norm, light1),0.0);

	float spotEffect1 = dot(-light1, spot1);
	if (spotEffect1 > gl_LightSource[1].spotCosCutoff) {
		spotEffect1 = pow(spotEffect1, gl_LightSource[1].spotExponent);
		diffuseColor1 = gl_LightSource[1].diffuse * spotEffect1;
		specularColor1 = gl_LightSource[1].specular * spotEffect1;
		ambientColor1 = gl_LightSource[1].ambient * spotEffect1;
		
		vec4 diffuse1 = diffuseColor1 * normDotLightDir1 * gl_FrontMaterial.diffuse;
		vec4 specular1 = specularColor1 * pow(max(dot(reflect1, i), 0.0), gl_FrontMaterial.shininess) * gl_FrontMaterial.specular;
		
		colorFrom1 += diffuse1 + specular1 + ambientColor1 * gl_FrontMaterial.ambient;

	}
	else {
		colorFrom1 += gl_LightSource[1].ambient * gl_FrontMaterial.ambient;
	}


	gl_FragColor = colorFrom0 + colorFrom1;
	gl_FragColor *= texture2D(tex, gl_TexCoord[0].st);
} 

