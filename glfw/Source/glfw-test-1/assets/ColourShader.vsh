#version 150

in vec3 aPositionVertex;
in vec3 aFragColor;
uniform mat4 MVP;


out vec3 inColor;

void main()
{
	inColor = aFragColor;
	gl_Position = MVP * vec4(aPositionVertex, 1.0);
}
