layout(triangles) in;
layout(triangle_strip, max_vertices = 96) out;

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
	
	for(int i = 0; i < gl_in.length(); i++){
	//front
		fragUV=vec2(1.0, 1.0);
		fragNormal = vec3(0.0, 0.0, 1.0);
		gl_Position = MVP*(gl_in[i].gl_Position.xyzw+vec4(0.05 ,-0.05 ,0.05 ,0.0));
		EmitVertex();
		fragUV=vec2(1.0,0.0);
		gl_Position = MVP*(gl_in[i].gl_Position.xyzw+vec4(0.05 ,0.05 ,0.05 ,0.0));
		EmitVertex();
		fragUV=vec2(0.0,1.0);
		gl_Position = MVP*(gl_in[i].gl_Position.xyzw+vec4(-0.05 ,-0.05 ,0.05 ,0.0));
		EmitVertex();
		fragUV=vec2(0.0,0.0);
		gl_Position = MVP*(gl_in[i].gl_Position.xyzw+vec4(-0.05 ,0.05 ,0.05 ,0.0));
		EmitVertex();
		EndPrimitive();
		
		//back
		fragNormal = vec3(0.0, 0.0, -1.0);
		fragUV=vec2(0.0,1.0);
		gl_Position = MVP*(gl_in[i].gl_Position.xyzw+vec4(0.05 ,-0.05 ,-0.05 ,0.0));
		EmitVertex();
		fragUV=vec2(0.0, 0.0);
		gl_Position = MVP*(gl_in[i].gl_Position.xyzw+vec4(0.05 ,0.05 ,-0.05 ,0.0));
		EmitVertex();
		fragUV=vec2(1.0,1.0);
		gl_Position = MVP*(gl_in[i].gl_Position.xyzw+vec4(-0.05 ,-0.05 ,-0.05 ,0.0));
		EmitVertex();
		fragUV=vec2(1.0,0.0);
		gl_Position = MVP*(gl_in[i].gl_Position.xyzw+vec4(-0.05 ,0.05 ,-0.05 ,0.0));
		EmitVertex();
		EndPrimitive();
		
		//left
		fragNormal = vec3(-1.0, 0.0, 0.0);
		fragUV=vec2(1.0,1.0);
		gl_Position = MVP*(gl_in[i].gl_Position.xyzw+vec4(-0.05 ,-0.05 ,0.05 ,0.0));
		EmitVertex();
		fragUV=vec2(1.0, 0.0);
		gl_Position = MVP*(gl_in[i].gl_Position.xyzw+vec4(-0.05 ,0.05 ,0.05 ,0.0));
		EmitVertex();
		fragUV=vec2(0.0,1.0);
		gl_Position = MVP*(gl_in[i].gl_Position.xyzw+vec4(-0.05 ,-0.05 ,-0.05 ,0.0));
		EmitVertex();
		fragUV=vec2(0.0,0.0);
		gl_Position = MVP*(gl_in[i].gl_Position.xyzw+vec4(-0.05 ,0.05 ,-0.05 ,0.0));
		EmitVertex();
		EndPrimitive();
		
		//right
		fragNormal = vec3(1.0, 0.0, 0.0);
		fragUV=vec2(0.0,1.0);
		gl_Position = MVP*(gl_in[i].gl_Position.xyzw+vec4(0.05 ,-0.05 ,0.05 ,0.0));
		EmitVertex();
		fragUV=vec2(0.0, 0.0);
		gl_Position = MVP*(gl_in[i].gl_Position.xyzw+vec4(0.05 ,0.05 ,0.05 ,0.0));
		EmitVertex();
		fragUV=vec2(1.0,1.0);
		gl_Position = MVP*(gl_in[i].gl_Position.xyzw+vec4(0.05 ,-0.05 ,-0.05 ,0.0));
		EmitVertex();
		fragUV=vec2(1.0,0.0);
		gl_Position = MVP*(gl_in[i].gl_Position.xyzw+vec4(0.05 ,0.05 ,-0.05 ,0.0));
		EmitVertex();
		EndPrimitive();
		
		//top
		fragNormal = vec3(0.0, 1.0, 0.0);
		fragUV=vec2(1.0,0.0);
		gl_Position = MVP*(gl_in[i].gl_Position.xyzw+vec4(0.05 ,0.05 ,-0.05 ,0.0));
		EmitVertex();
		fragUV=vec2(1.0, 1.0);
		gl_Position = MVP*(gl_in[i].gl_Position.xyzw+vec4(0.05 ,0.05 ,0.05 ,0.0));
		EmitVertex();
		fragUV=vec2(0.0,0.0);
		gl_Position = MVP*(gl_in[i].gl_Position.xyzw+vec4(-0.05 ,0.05 ,-0.05 ,0.0));
		EmitVertex();
		fragUV=vec2(0.0,1.0);
		gl_Position = MVP*(gl_in[i].gl_Position.xyzw+vec4(-0.05 ,0.05 ,0.05 ,0.0));
		EmitVertex();
		EndPrimitive();
		
		//bottom
		fragNormal = vec3(0.0, -1.0, 0.0);
		fragUV=vec2(0.0,0.0);
		gl_Position = MVP*(gl_in[i].gl_Position.xyzw+vec4(0.05 ,-0.05 ,-0.05 ,0.0));
		EmitVertex();
		fragUV=vec2(0.0, 1.0);
		gl_Position = MVP*(gl_in[i].gl_Position.xyzw+vec4(0.05 ,-0.05 ,0.05 ,0.0));
		EmitVertex();
		fragUV=vec2(1.0,0.0);
		gl_Position = MVP*(gl_in[i].gl_Position.xyzw+vec4(-0.05 ,-0.05 ,-0.05 ,0.0));
		EmitVertex();
		fragUV=vec2(1.0,1.0);
		gl_Position = MVP*(gl_in[i].gl_Position.xyzw+vec4(-0.05 ,-0.05 ,0.05 ,0.0));
		EmitVertex();
		EndPrimitive();
	}

}