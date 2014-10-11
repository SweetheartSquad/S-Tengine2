#version 150

uniform mat4 model;

uniform struct Light{
	vec3 position;
	vec3 intensities;
};

uniform Light lights[50];
uniform int numLights;

in vec3 fragVert;
in vec3 fragNormal;
in vec4 fragColor;

out vec4 outColor;

void main()
{
    mat3 normalMatrix = transpose(inverse(mat3(model)));
	vec3 normal = normalize(normalMatrix * fragNormal);

	vec3 fragWorldPosition = vec3(model * vec4(fragVert, 1));

	float brightness;
	vec3 outIntensities;

	for(int i = 0; i < numLights; i++){
		vec3 surfaceToLight = lights[i].position -fragWorldPosition;
		brightness += dot(normal, surfaceToLight) / (length(surfaceToLight) * length(normal));
		outIntensities += vec3(lights[i].intensities);
	}
	
	brightness = clamp(brightness, 0, 1);
 
	outColor = vec4(brightness * vec3(outIntensities), 1) * fragColor;
}