#version 150

layout(points) in;
layout(triangle_strip, max_vertices = 24) out;

in vec3 fragVertGeo[];
in vec3 fragNormalGeo[];
in vec4 fragColorGeo[];
in vec2 fragUVGeo[];

out vec3 fragVert;
out vec3 fragNormal;
out vec4 fragColor;
out vec2 fragUV;

uniform mat4 M;
uniform mat4 VP;
uniform float resolution;

void rotate(in int i, inout vec3 inVec){
	if(i == 0){
		inVec = inVec;
	}else if(i == 1){
		inVec = vec3(-inVec.y, inVec.x, inVec.z);
	}else if(i == 2){
		inVec = vec3(-inVec.z, inVec.y, inVec.x);
	}
}

void main() {

	fragVert = fragVertGeo[0];
	fragNormal = fragNormalGeo[0];
	fragColor = fragColorGeo[0];

	for(int i = 0; i < 3; ++i){
		for(int x = 0; x <= 1; ++x){
			
			fragNormal = vec3((x-0.5)*2, 0.0, 0);
			rotate(i, fragNormal);

			for(int y = 0; y <= 1; ++y){
				for(int z = 0; z <= 1; ++z){
					fragUV = vec2(y, z);
					
					vec3 cubeVert = vec3(x-0.5, y-0.5, z-0.5) * resolution;
					rotate(i, cubeVert);

					
					vec4 pos = M * gl_in[0].gl_Position.xyzw;

					pos.xyz = pos.xyz - mod(pos.xyz, vec3(resolution, resolution, resolution));
					
					gl_Position = VP * (pos + vec4(cubeVert, 0.0));
					
					EmitVertex();
				}
			}
			EndPrimitive();
		}
	}
}