#version 150

out vec4 fragmentdepth;

void main(){
	//float depth = gl_FragCoord.z;
	//float dx = dFdx(depth);
	//float dy = dFdy(depth);
	//fragmentdepth = vec4(depth, pow(depth, 2.0) + 0.25*(dx*dx + dy*dy), 0.0, 1.0);
	gl_FragColor = vec4(0, 0, depth, 1);
	fragmentdepth = vec4(1.5f, 0, 0, 1);
}