layout(points) in;
layout(triangle_strip, max_vertices = 32) out;

in vec3 fragVertGeo[];
in vec3 fragNormalGeo[];
in vec4 fragColorGeo[];
in vec2 fragUVGeo[];

out vec3 fragVert;
out vec3 fragNormal;
out vec4 fragColor;
out vec2 fragUV;


uniform mat4 MVP;

void main() {

	fragVert = fragVertGeo[0];
	fragNormal = fragNormalGeo[0];
	fragColor = fragColorGeo[0];
	fragUV = fragUVGeo[0];

	//front
	fragUV=vec2(1.0, 1.0);
	gl_Position = MVP*(gl_in[0].gl_Position.xyzw+vec4(0.05,-0.05,0.05,0.0));
	EmitVertex();
	fragUV=vec2(1.0,0.0);
	gl_Position = MVP*(gl_in[0].gl_Position.xyzw+vec4(0.05,0.05,0.05,0.0));
	EmitVertex();
	fragUV=vec2(0.0,1.0);
	gl_Position = MVP*(gl_in[0].gl_Position.xyzw+vec4(-0.05,-0.05,0.05,0.0));
	EmitVertex();
	fragUV=vec2(0.0,0.0);
	gl_Position = MVP*(gl_in[0].gl_Position.xyzw+vec4(-0.05,0.05,0.05,0.0));
	EmitVertex();
	EndPrimitive();
		
	//back
	fragUV=vec2(0.0,1.0);
	gl_Position = MVP*(gl_in[0].gl_Position.xyzw+vec4(0.05,-0.05,-0.05,0.0));
	EmitVertex();
	fragUV=vec2(0.0, 0.0);
	gl_Position = MVP*(gl_in[0].gl_Position.xyzw+vec4(0.05,0.05,-0.05,0.0));
	EmitVertex();
	fragUV=vec2(1.0,1.0);
	gl_Position = MVP*(gl_in[0].gl_Position.xyzw+vec4(-0.05,-0.05,-0.05,0.0));
	EmitVertex();
	fragUV=vec2(1.0,0.0);
	gl_Position = MVP*(gl_in[0].gl_Position.xyzw+vec4(-0.05,0.05,-0.05,0.0));
	EmitVertex();
	EndPrimitive();
		
	//left
	fragUV=vec2(1.0,1.0);
	gl_Position = MVP*(gl_in[0].gl_Position.xyzw+vec4(-0.05,-0.05,0.05,0.0));
	EmitVertex();
	fragUV=vec2(1.0, 0.0);
	gl_Position = MVP*(gl_in[0].gl_Position.xyzw+vec4(-0.05,0.05,0.05,0.0));
	EmitVertex();
	fragUV=vec2(0.0,1.0);
	gl_Position = MVP*(gl_in[0].gl_Position.xyzw+vec4(-0.05,-0.05,-0.05,0.0));
	EmitVertex();
	fragUV=vec2(0.0,0.0);
	gl_Position = MVP*(gl_in[0].gl_Position.xyzw+vec4(-0.05,0.05,-0.05,0.0));
	EmitVertex();
	EndPrimitive();
		
	//right
	fragUV=vec2(0.0,1.0);
	gl_Position = MVP*(gl_in[0].gl_Position.xyzw+vec4(0.05,-0.05,0.05,0.0));
	EmitVertex();
	fragUV=vec2(0.0, 0.0);
	gl_Position = MVP*(gl_in[0].gl_Position.xyzw+vec4(0.05,0.05,0.05,0.0));
	EmitVertex();
	fragUV=vec2(1.0,1.0);
	gl_Position = MVP*(gl_in[0].gl_Position.xyzw+vec4(0.05,-0.05,-0.05,0.0));
	EmitVertex();
	fragUV=vec2(1.0,0.0);
	gl_Position = MVP*(gl_in[0].gl_Position.xyzw+vec4(0.05,0.05,-0.05,0.0));
	EmitVertex();
	EndPrimitive();
		
	//top
	fragUV=vec2(1.0,0.0);
	gl_Position = MVP*(gl_in[0].gl_Position.xyzw+vec4(0.05,0.05,-0.05,0.0));
	EmitVertex();
	fragUV=vec2(1.0, 1.0);
	gl_Position = MVP*(gl_in[0].gl_Position.xyzw+vec4(0.05,0.05,0.05,0.0));
	EmitVertex();
	fragUV=vec2(0.0,0.0);
	gl_Position = MVP*(gl_in[0].gl_Position.xyzw+vec4(-0.05,0.05,-0.05,0.0));
	EmitVertex();
	fragUV=vec2(0.0,1.0);
	gl_Position = MVP*(gl_in[0].gl_Position.xyzw+vec4(-0.05,0.05,0.05,0.0));
	EmitVertex();
	EndPrimitive();
		
	//bottom
	fragUV=vec2(0.0,0.0);
	gl_Position = MVP*(gl_in[0].gl_Position.xyzw+vec4(0.05,-0.05,-0.05,0.0));
	EmitVertex();
	fragUV=vec2(0.0, 1.0);
	gl_Position = MVP*(gl_in[0].gl_Position.xyzw+vec4(0.05,-0.05,0.05,0.0));
	EmitVertex();
	fragUV=vec2(1.0,0.0);
	gl_Position = MVP*(gl_in[0].gl_Position.xyzw+vec4(-0.05,-0.05,-0.05,0.0));
	EmitVertex();
	fragUV=vec2(1.0,1.0);
	gl_Position = MVP*(gl_in[0].gl_Position.xyzw+vec4(-0.05,-0.05,0.05,0.0));
	EmitVertex();
	EndPrimitive();

}