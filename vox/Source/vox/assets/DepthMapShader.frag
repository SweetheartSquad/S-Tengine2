#version 150
#extension GL_ARB_explicit_attrib_location : enable

layout(location = 0) out vec4 fragmentdepth;

void main(){

	fragmentdepth = vec4(gl_FragCoord.z, 0, 0, 1);
}