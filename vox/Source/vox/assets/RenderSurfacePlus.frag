#version 150
in vec2 Texcoord;
out vec4 outColor;
uniform sampler2D texFramebuffer;
uniform int distortion;
uniform float time;

// weird function that everyone uses online I guess
float rand(vec2 co){
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

vec2 noiseEdges(vec2 uv){
	float amount = 0.75;
	vec2 olduv = uv;
	uv -= vec2(0.5, 0.5);
	float mag = max(0, -amount + length(abs(uv)*2));
	uv.x = rand(uv*time);
	uv.y = rand(uv*time);
	return olduv + (uv - olduv)*mag;
}
vec2 wave(vec2 uv){
	uv.x += sin(uv.y * 4*2*3.14159 + time) / 100;
	return uv;
}

vec2 kaleidoGrid(vec2 uv){
	if(int(uv.x*19) % 2 < 0.5){
		uv.x = 1 - uv.x;
	}
	if(int(uv.y*10) % 2 < 0.5){
		uv.y += (2-uv.y)/2;
		uv.y = 1 - uv.y;
	}
	vec2 centered = (2.0 * uv - 1);
	uv /= vec2(19, 10);
	float speed = 2;
	float len = length(centered);
	uv = uv + (centered/len)*cos(len*12 - time*speed)*0.03;
	return uv * vec2(19, 10);
	/*float speed = 0.3;
	uv.x = (uv.x-0.5 + cos(time*speed));
	uv.y = (uv.y-0.5 + sin(time*speed));
	return uv;*/
}
vec2 tile(vec2 uv){
	float speed = 0.3;
	uv.x = cos(uv.x + time*speed);
	uv.y = cos(uv.y + time*speed);
	return uv;
}
vec2 gridOffset(vec2 uv){
	if(int(uv.x*192) % 2 < 0.5){
		uv.x = 1 - uv.x;
	}
	if(int(uv.y*108) % 2 < 0.5){
		uv.y = 1 - uv.y;
	}
	return uv;
}

vec2 fishEye(vec2 uv){
	vec2 olduv = uv;
	float amount = 0.4;
	uv -= vec2(0.5, 0.5);
	float mag = max(0, -amount + length(abs(uv)*2));
	uv += vec2(0.5, 0.5);
	uv /= 2;

	return olduv + (uv - olduv)*mag;
}

void main() {
	vec2 uv;
	switch(distortion){
	default:
	case 0:
		uv = Texcoord;
		break;
	case 1:
		uv = gridOffset(Texcoord);
		break;
	case 2:
		uv = fishEye(Texcoord);
		break;
	case 3:
		uv = tile(Texcoord);
		break;
	case 4:
		uv = kaleidoGrid(Texcoord);
		break;
	case 5:
		uv = noiseEdges(Texcoord);
		break;
	case 6:
		uv = wave(Texcoord);
		break;
	}
    outColor = vec4(texture(texFramebuffer, uv));
}