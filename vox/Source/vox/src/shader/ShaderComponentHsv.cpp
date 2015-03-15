#pragma once

#include <shader\ShaderComponentHsv.h>
#include <shader\ShaderVariables.h>
#include <GLUtils.h>
#include <RenderOptions.h>
#include <shader\Shader.h>

#include <glew\glew.h>


ShaderComponentHsv::ShaderComponentHsv(Shader * _shader, float _hue, float _saturation, float _value, float _alpha):
	ShaderComponent(_shader),
	hue(_hue),
	saturation(_saturation),
	value(_value),
	alpha(_alpha)
{
}

ShaderComponentHsv::~ShaderComponentHsv(){
}
	
std::string ShaderComponentHsv::getVertexVariablesString(){
	return DEFINE + SHADER_COMPONENT_HSV + ENDL;
}

std::string ShaderComponentHsv::getFragmentVariablesString(){
	return 
		DEFINE + SHADER_COMPONENT_HSV + ENDL +
		"uniform float " + GL_UNIFORM_ID_HUE + SEMI_ENDL + 
		"uniform float " + GL_UNIFORM_ID_SATURATION + SEMI_ENDL + 
		"uniform float " + GL_UNIFORM_ID_VALUE + SEMI_ENDL +
		"uniform float " + GL_UNIFORM_ID_ALPHA + SEMI_ENDL;
}

std::string ShaderComponentHsv::getVertexBodyString(){
	return "";
}

std::string ShaderComponentHsv::getFragmentBodyString(){
	return "";
}

std::string ShaderComponentHsv::getOutColorMod(){
	return
		"// convert current outColor to HSV\n"
		"vec4 hsvOutColor = outColor" + SEMI_ENDL + 
		"{vec4 hsvK = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0)" + SEMI_ENDL + 
		"vec4 hsvP = mix(vec4(hsvOutColor.bg, hsvK.wz), vec4(hsvOutColor.gb, hsvK.xy), step(hsvOutColor.b, hsvOutColor.g))" + SEMI_ENDL +
		"vec4 hsvQ = mix(vec4(hsvP.xyw, hsvOutColor.r), vec4(hsvOutColor.r, hsvP.yzx), step(hsvP.x, hsvOutColor.r))" + SEMI_ENDL +
		"float hsvD = hsvQ.x - min(hsvQ.w, hsvQ.y)" + SEMI_ENDL +
		"float hsvE = 1.0e-10" + SEMI_ENDL +
		"hsvOutColor = vec4(abs(hsvQ.z + (hsvQ.w - hsvQ.y) / (6.0 * hsvD + hsvE)), hsvD / (hsvQ.x + hsvE), hsvQ.x, 1)" + SEMI_ENDL +
		"}" + SEMI_ENDL +

		"// modify hsv here\n"
		"hsvOutColor.r += " + GL_UNIFORM_ID_HUE + SEMI_ENDL +
		"hsvOutColor.g *= " + GL_UNIFORM_ID_SATURATION + SEMI_ENDL +
		"hsvOutColor.b *= " + GL_UNIFORM_ID_VALUE + SEMI_ENDL +

		"// convert back\n"
		"{vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0)" + SEMI_ENDL + 
		"vec3 p = abs(fract(hsvOutColor.xxx + K.xyz) * 6.0 - K.www)" + SEMI_ENDL + 
		"vec3 rgbOutColor =  hsvOutColor.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), hsvOutColor.y)" + SEMI_ENDL + 
		GL_OUT_OUT_COLOR + " = vec4(rgbOutColor.rgb, outColor.w * " + GL_UNIFORM_ID_ALPHA + ")" + SEMI_ENDL +
		"}" + SEMI_ENDL;

}

void ShaderComponentHsv::configureUniforms(vox::MatrixStack * _matrixStack, RenderOptions * _renderOption, NodeRenderable * _nodeRenderable){
	while(hue > 1.f){
		hue -= 1.f;
	}
	glUniform1f(glGetUniformLocation(_renderOption->shader->getProgramId(), GL_UNIFORM_ID_HUE.c_str()), hue);
	glUniform1f(glGetUniformLocation(_renderOption->shader->getProgramId(), GL_UNIFORM_ID_SATURATION.c_str()), saturation);
	glUniform1f(glGetUniformLocation(_renderOption->shader->getProgramId(), GL_UNIFORM_ID_VALUE.c_str()), value);
	glUniform1f(glGetUniformLocation(_renderOption->shader->getProgramId(), GL_UNIFORM_ID_ALPHA.c_str()), alpha);
}

void ShaderComponentHsv::setHue(float _hue){
	hue = _hue;
	makeDirty();
}
void ShaderComponentHsv::setSaturation(float _sat){
	saturation = _sat;
	makeDirty();
}
void ShaderComponentHsv::setValue(float _val){
	value = _val;
	makeDirty();
}
float ShaderComponentHsv::getHue(){
	return hue;
}
float ShaderComponentHsv::getSaturation(){
	return saturation;
}
float ShaderComponentHsv::getValue(){
	return value;
}