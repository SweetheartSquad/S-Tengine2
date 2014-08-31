#version 150

in vec4 colorForFrag;

out vec4 outColor;

void main()
{
    outColor = colorForFrag;
}