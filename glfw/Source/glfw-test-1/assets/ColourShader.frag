#version 150

//in vec4 colorForFrag;
in vec2 outUV;

out vec3 outColor;
uniform sampler2D textureSampler;

void main()
{
     outColor = texture(textureSampler, outUV).rgb;
}