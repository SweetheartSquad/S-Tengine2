#version 150

//in vec4 colorForFrag;
in vec2 outUV;

out vec3 outColor;
//Just define 32 samplers for now. This is are max number of textures
uniform sampler2D textureSampler[32];
uniform int numTextures;

void main()
{
	 for(int i = 0; i < numTextures; i++){
		if(i == 0){
			outColor = texture(textureSampler[i], outUV).rgb;
		}else{
			outColor = mix(outColor, texture(textureSampler[i], outUV).rgb, 0.5);
		}
	 }
}