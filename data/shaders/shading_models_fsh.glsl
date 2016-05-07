vec4 lambert(vec3 normal, vec3 lightDir, vec4 diffuseColor)
{
	return diffuseColor * max(dot(normal, lightDir), 0.0);
}

vec4 wrapAroud(vec3 normal, vec3 lightDir, vec4 diffuseColor, float factor)
{
	return diffuseColor * max(dot(normal, lightDir) + factor, 0.0) / ( 1.0 + factor );
}

vec4 phong(vec3 normal, vec3 lightDir, vec3 viewDir, vec4 diffuseColor, vec4 specularColor, float specPower)
{
	vec3 r	= reflect(-viewDir, normal);
	vec4 diffuse = diffuseColor * max(dot(normal, lightDir), 0.1);
	vec4 specular = specularColor * pow( max(dot(lightDir, r), 0.0), specPower);

	return diffuse + specular;
}

vec4 blinn(vec3 normal, vec3 lightDir, vec3 viewDir, vec4 diffuseColor, vec4 specularColor, float specPower)
{
	vec3 h = normalize(lightDir + viewDir);
	vec4 diffuse = diffuseColor * max (dot(normal, lightDir), 0.1);
	vec4 specular = specularColor * pow(max(dot(normal, h), 0.0), specPower);

	return diffuse + specular;
}