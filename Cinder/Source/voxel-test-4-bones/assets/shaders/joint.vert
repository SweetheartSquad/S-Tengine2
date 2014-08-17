#version 110

varying vec3 faceNormals;

void main()
{
	faceNormals = gl_Normal*0.5+vec3(0.5,0.5,0.5);

	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();
	gl_FrontColor = gl_Color;
}
