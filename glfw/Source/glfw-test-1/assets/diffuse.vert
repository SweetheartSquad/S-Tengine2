#version 150
#extension GL_ARB_explicit_attrib_location : enable

layout(location = 1) in vec3 aVertexPosition;
layout(location = 2) in vec4 aVertexColor;
layout(location = 3) in vec3 aVertexNormals;

uniform mat4 MVP;

out vec3 fragVert;
out vec3 fragNormal;
out vec4 fragColor;

void main(){
	fragVert = aVertexPosition;
	fragNormal = aVertexNormals;
	fragColor = aVertexColor;
	gl_Position = MVP * vec4(aVertexPosition, 1.0);
}
