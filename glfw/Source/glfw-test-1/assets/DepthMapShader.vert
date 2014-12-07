#version 150
#extension GL_ARB_explicit_attrib_location : enable

layout(location = 1) in vec3 aVertexPosition;

uniform mat4 depthMVP;


void main(){
	gl_Position = depthMVP * vec4(aVertexPosition, 1.0); 
}