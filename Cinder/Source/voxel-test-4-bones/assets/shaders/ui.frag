#version 110

uniform vec3 pickingColor;

void main(){		
	// write final color in first color target 
	gl_FragData[0] = gl_Color;

	// write picking color code in second color target
	gl_FragData[1] = vec4(pickingColor, 1.0);
}