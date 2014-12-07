#version 150

uniform mat4 model;

struct Light{
	vec3 position;
	vec3 intensities;
	float ambientCoefficient;
	float attenuation;
};

struct Material{
	float shininess;
	vec3 specularColor;
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
	vec4 fragColorTex = vec4(0,0,0,1);

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
	vec3 surfaceToCamera = fragVert - fragWorldPosition;
	
	outColorTemp = vec4(0,0,0,1);

	for(int i = 0; i < numLights; i++){
		for(int j = 0; j < numMaterials; j++){
			vec3 surfaceToLight = normalize(lights[i].position - fragWorldPosition);
			
			//ambient
			vec3 ambient = lights[i].ambientCoefficient * fragColorTex.rgb * lights[i].intensities;
		
			//diffuse
			float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
			vec3 diffuse = diffuseCoefficient * fragColorTex.rgb * lights[i].intensities;
			diffuse = clamp(diffuse, 0.0, 1.0);
		
			//specular
			float specularCoefficient = 0.0;
			//only calculate specular for the front side of the surface
			if(diffuseCoefficient > 0.0){
				vec3 lightDirection = normalize(-surfaceToLight);
				vec3 viewDirection = normalize(-surfaceToCamera);
				vec3 halfAngle = normalize(lightDirection + viewDirection);

				specularCoefficient = pow(max(0.0, dot(halfAngle, normal)), materials[j].shininess);
			}
			vec3 specular = specularCoefficient * materials[j].specularColor * lights[i].intensities;
			//specular = clamp(specular, 0.0, 1.0);
			
			//attenuation
			float distanceToLight = length(lights[i].position - fragWorldPosition);
			float attenuation = 1.0 / (1.0 + lights[i].attenuation * pow(distanceToLight, 2));
		
			//linear color (color before gamma correction)
			vec3 linearColor = ambient + attenuation * (diffuse + specular);
    
			//final color (after gamma correction)
			vec3 gamma = vec3(1.0/2.2, 1.0/2.2, 1.0/2.2);
			vec3 gammaColor = pow(linearColor, gamma);
			outColorTemp = outColorTemp + vec4(gammaColor, 1);
		}
	}

	outColor = outColorTemp;
}