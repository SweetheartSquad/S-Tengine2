#version 150

uniform mat4 model;

uniform struct Light{
	vec3 position;
	vec3 intensities;
};

uniform struct Material{
	int type;
};

uniform Light lights[5];
uniform int numLights;

uniform sampler2D textureSampler[5];
uniform int numTextures;

uniform Material materials[5];
uniform int numMaterials;

in vec3 fragVert;
in vec3 fragNormal;
in vec4 fragColor;
in vec2 fragUV;

out vec4 outColor;

void main()
{
	vec4 fragColorTex = vec4(0,0,0,0);

	if(numTextures == 0){
		fragColorTex = fragColor;
	}

	for(int i = 0; i < numTextures; i++){
		if(i == 0){
			fragColorTex = texture(textureSampler[i], fragUV).rgba;
		}else{
			fragColorTex = mix(fragColorTex, texture(textureSampler[i], fragUV).rgba, 0.5);
		}
	}

    mat3 normalMatrix = transpose(inverse(mat3(model)));

	vec3 normal = normalize(normalMatrix * fragNormal);

	vec3 fragWorldPosition = vec3(model * vec4(fragVert, 1));

	float brightness = 0;
	vec3 outIntensities = vec3(0,0,0);

	for(int i = 0; i < numLights; i++){
		vec3 surfaceToLight = lights[i].position - fragWorldPosition;
		brightness += dot(normal, surfaceToLight) / (length(surfaceToLight) * length(normal));
		outIntensities += vec3(lights[i].intensities);
	}
	
	brightness = clamp(brightness, 0.1, 1);
 
	outColor = vec4(brightness * vec3(outIntensities), 1) * fragColorTex;
}