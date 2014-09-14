#version 150

//in vec4 colorForFrag;
in vec2 outUV;

out vec3 outColor;
uniform sampler2D tex;

void main()
{
     outColor = texture(tex, outUV).rgb;
}