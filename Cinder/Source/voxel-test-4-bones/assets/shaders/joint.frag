#version 120

uniform vec3 pickingColor;

varying vec3 faceNormals;
varying vec3 vVertexPosition;

uniform bool availableForVoxel;

void main(){		
	// write final color in first color target 
	gl_FragData[0] = gl_Color * vec4(faceNormals,1);

	// write picking color code in second color target
	gl_FragData[1] = vec4(pickingColor, 1.0);
	
	// write normals in third color target
	gl_FragData[2] = vec4(faceNormals,1.0);
	
	// write position in fourth color target
	gl_FragData[3] = vec4(availableForVoxel ? vVertexPosition : vec3(0.f, 0.f, 0.f), 1.0f);
}