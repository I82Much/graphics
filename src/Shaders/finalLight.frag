// Basic fragment shader for 2D texture mapping.

// Define a variable to access the texture. 
uniform sampler2D tex;

varying vec3 normal, eye; // 6 components
varying vec4 vertCoords;

// assumes norm and i are normalized and computes the color from the
// spot light at index "index"
vec4 colorFromSpots (vec3 norm, vec3 i, vec4 vertCorods, int index) {

	vec3 lightDir = normalize(vec3(gl_LightSource[index].position - vertCoords));
	vec3 reflection = normalize(-lightDir - (2.0 * max(0.0, dot(norm, lightDir)) * norm));

	vec4 diffuseColor = vec4(0,0,0,0);
	vec4 specularColor = vec4(0,0,0,0);
	vec4 ambientColor = vec4(0,0,0,0);
	vec4 colorFrom = vec4(0.0,0.0,0.0,1.0);

	vec3 spot = normalize(gl_LightSource[index].spotDirection);

	float normDotLightDir = max(dot(norm, lightDir),0.0);

	float spotEffect = dot(-lightDir, spot);
	if (spotEffect > gl_LightSource[index].spotCosCutoff) {
		spotEffect = pow(spotEffect, gl_LightSource[index].spotExponent);
		diffuseColor = gl_LightSource[index].diffuse * spotEffect;
		specularColor = gl_LightSource[index].specular * spotEffect;
		ambientColor = gl_LightSource[index].ambient * spotEffect;
		
		vec4 diffuse = diffuseColor * normDotLightDir * gl_FrontMaterial.diffuse;
		vec4 specular = specularColor * pow(max(dot(reflection, i), 0.0), gl_FrontMaterial.shininess) * gl_FrontMaterial.specular;
		
		colorFrom += diffuse + specular + ambientColor * gl_FrontMaterial.ambient;

	}
	else {
		colorFrom += gl_LightSource[index].ambient * gl_FrontMaterial.ambient;
	}
	
	return colorFrom;

} // end colorFromSpots


// assumes norm and i are normalized and computes the color of the point
// light at index "index"
vec4 colorFromPoints (vec3 norm, vec3 i, vec4 vertCoords, int index) {

	vec3 lightDir = normalize(vec3(gl_LightSource[index].position - vertCoords));
	vec3 reflection = -normalize(lightDir - (2.0 * max(0.0, dot(norm, lightDir)) * norm));
	float lengthVal = length(vec3(gl_LightSource[index].position - vertCoords));


	vec4 diffuseColor = gl_LightSource[index].diffuse / pow(lengthVal, 2.0);
	vec4 specularColor = gl_LightSource[index].specular / pow(lengthVal, 2.0);
	vec4 ambientColor = gl_LightSource[index].ambient / pow(lengthVal, 2.0);

	float normDotLightDir = max(dot(norm, lightDir),0.0);

	vec4 diffuse = diffuseColor * normDotLightDir * gl_FrontMaterial.diffuse;
	vec4 specular = specularColor * pow(max(dot(reflection, i), 0.0), gl_FrontMaterial.shininess) * gl_FrontMaterial.specular;
		
	vec4 colorFrom = diffuse + specular + ambientColor * gl_FrontMaterial.ambient;

	return colorFrom;

} // end colorFromPoints




void main() {		
	// The built-in function texture2D performs the texture
	// look-up. We read the texture coordinates from the
	// pre-defined varying variable gl_TexCoord[0], which
	// we set in the vertex shader.


	vec3 norm = normalize(normal);
	vec3 i = normalize(eye);

	vec4 colorFrom0 = colorFromSpots(norm, i, vertCoords, 0);
	vec4 colorFrom1 = colorFromSpots(norm, i, vertCoords, 1);
	vec4 colorFrom2 = colorFromPoints(norm, i, vertCoords, 2);
	vec4 colorFrom3 = colorFromPoints(norm, i, vertCoords, 3);
	vec4 colorFrom4 = colorFromPoints(norm, i, vertCoords, 4);
	vec4 colorFrom5 = colorFromPoints(norm, i, vertCoords, 5);
	vec4 colorFrom6 = colorFromPoints(norm, i, vertCoords, 6);
	vec4 colorFrom7 = colorFromPoints(norm, i, vertCoords, 7);

	gl_FragColor = colorFrom0;
	gl_FragColor += colorFrom1;
	gl_FragColor += colorFrom2;
	gl_FragColor += colorFrom3;
	gl_FragColor += colorFrom4;
	gl_FragColor += colorFrom5;
	gl_FragColor += colorFrom6;
	gl_FragColor += colorFrom7;
	gl_FragColor *= texture2D(tex, gl_TexCoord[0].st);

}  // end main

