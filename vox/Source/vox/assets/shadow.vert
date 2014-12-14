#version 150
#extension GL_ARB_explicit_attrib_location : enable

layout(location = 1) in vec2 aVertexPosition;
layout(location = 2) in vec2 aVertexUVs;
out vec2 Texcoord;
void main() {
    Texcoord = aVertexUVs;
    gl_Position = vec4(aVertexPosition, 0.0, 1.0);
}