#version 150

uniform mat4 model;

uniform struct Light{
	vec3 position;
	vec3 intensities;
} light;

in vec3 fragVert;
in vec3 fragNormal;
in vec4 fragColor;

out vec4 outColor;

void main()
{
    mat3 normalMatrix = transpose(inverse(mat3(model)));
	vec3 normal = normalize(normalMatrix * fragNormal);

	vec3 fragWorldPosition = vec3(model * vec4(fragVert, 1));

	vec3 surfaceToLight = light.position -fragWorldPosition;

	float brightness = dot(normal, surfaceToLight) / (length(surfaceToLight) * length(normal));
	brightness = clamp(brightness, 0, 1);
   
    outColor = brightness * vec4(light.intensities, 1) * fragColor;
}