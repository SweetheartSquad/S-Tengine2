#version 110

varying vec3 v;
varying vec3 N;
varying vec3 faceNormals;

void main()
{
	v = vec3(gl_ModelViewMatrix * gl_Vertex);       
	N = normalize(gl_NormalMatrix * gl_Normal);
	faceNormals = gl_Normal*0.5+vec3(0.5,0.5,0.5);

	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();
}
