#version 110

uniform vec3 pickingColor;
uniform sampler2D myTexture1;
uniform bool tex;

void main(){		
   vec4 texval1 = texture2D(myTexture1, vec2(gl_TexCoord[0]));
	// write final color in first color target 
   if(tex){
	gl_FragData[0] = tex ? texval1 : gl_Color;
   }else{
	gl_FragData[0] = gl_Color;
   }
	// write picking color code in second color target
	gl_FragData[1] = vec4(pickingColor, 1.0);
}