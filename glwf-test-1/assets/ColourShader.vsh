#version 150

in vec3 aPositionVertex;
in vec3 aFragColor;
out vec3 inColor;

void main()
{
	inColor = aFragColor;
	gl_Position = gl_ModelViewProjectionMatrix * vec4(aPositionVertex, 1.0);

}
