// Basic fragment shader for 2D texture mapping.


varying vec3 lightDir2, reflection2; // 6 components
varying vec3 lightDir3, reflection3; // 6 components
//varying vec3 lightDir4, reflection4;
//varying vec3 lightDir5, reflection5;
//varying vec3 lightDir6, reflection6;
//varying vec3 lightDir7, reflection7;
varying float length2, length3;//, length4;//, length5, length6, length7;

vec4 pointLightColor (vec3 norm, vec3 i, vec3 lightDir, vec3 reflection, int index, float lengthVal) {
	vec3 light = normalize(lightDir);
	vec3 reflectVec = normalize(reflection);

	vec4 diffuseColor = gl_LightSource[index].diffuse / pow(lengthVal, 2.0);
	vec4 specularColor = gl_LightSource[index].specular / pow(lengthVal, 2.0);
	vec4 ambientColor = gl_LightSource[index].ambient / pow(lengthVal, 2.0);

	float normDotLightDir = max(dot(norm, light),0.0);

	vec4 diffuse = diffuseColor * normDotLightDir * gl_FrontMaterial.diffuse;
	vec4 specular = specularColor * pow(max(dot(reflectVec, i), 0.0), gl_FrontMaterial.shininess) * gl_FrontMaterial.specular;
		
	vec4 colorFrom = diffuse + specular + ambientColor * gl_FrontMaterial.ambient;

	return colorFrom;

}

vec4 colorFromPoints (vec3 norm, vec3 i) {		
	// gl_LightSource[2] is a point light
	vec4 colorFrom2 = pointLightColor(norm, i, lightDir2, reflection2, 2, length2);

	// gl_LightSource[3] is a point light
	vec4 colorFrom3 = pointLightColor(norm, i, lightDir3, reflection3, 3, length3);

	// gl_LightSource[4] is a point light
//	vec4 colorFrom4 = pointLightColor(norm, i, lightDir4, reflection4, 4, length4);

/*
	// gl_LightSource[5] is a point light
	vec4 colorFrom5 = pointLightColor(norm, i, lightDir5, reflection5, 5, length5);


	// gl_LightSource[6] is a point light
	vec4 colorFrom6 = pointLightColor(norm, i, lightDir6, reflection6, 6, length6);

	// gl_LightSource[7] is a point light
	vec4 colorFrom7 = pointLightColor(norm, i, lightDir7, reflection7, 7, length7);
*/

	vec4 returnColor = colorFrom2;
	returnColor += colorFrom3;
//	returnColor += colorFrom4;
//	returnColor += colorFrom5;
//	returnColor += colorFrom6;
//	returnColor += colorFrom7;
	
	return returnColor;

} 

