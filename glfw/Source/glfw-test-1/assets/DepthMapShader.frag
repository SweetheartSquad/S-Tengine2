#version 330 core

uniform mat4 model;

uniform struct Light{
	vec3 position;
	vec3 intensities;
};


uniform struct Material{
	int type;
};

uniform mat4 M;
uniform mat4 VP;

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

out float fragmentdepth;

void main(){
	float depth = gl_FragCoord.z;
	float dx = dFdx(depth);
	float dy = dFdy(depth);
	fragmentdepth = vec4(depth, pow(depth, 2.0) + 0.25*(dx*dx + dy*dy), 0.0, 1.0);
	//fragmentdepth = gl_FragCoord.z;
}