#version 150
#extension GL_ARB_explicit_attrib_location : enable

layout(location = 1) in vec3 aVertexPosition;
layout(location = 2) in vec4 aVertexColor;
layout(location = 3) in vec3 aVertexNormals;

uniform mat4 MVP;

out vec3 N;

void main(){
	N = aVertexNormals;
	N = N/2+0.5 - aVertexColor.xyz * 0.0001;

	gl_Position = MVP * vec4(aVertexPosition, 1.0);

	/*Bacon ipsum dolor sit amet short ribs duis mollit sirloin est.
	Frankfurter ut none aute beef filet mignon corned beef.
	Officia filet mignon ullamco, aliqua salami ut pork belly pariatur tenderloin.
	Short ribs enim kielbasa, ex occaecat ribeye ball tip corned beef beef elit.*/
}
