#version 110

varying vec3 faceNormals;
varying vec3 vVertexPosition;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	faceNormals = gl_Normal*0.5+vec3(0.5,0.5,0.5);

	gl_TexCoord[0] = gl_MultiTexCoord0;
	vVertexPosition = (modelMatrix * gl_Vertex).xyz;
	gl_Position = ftransform();
	gl_FrontColor = gl_Color;
}
