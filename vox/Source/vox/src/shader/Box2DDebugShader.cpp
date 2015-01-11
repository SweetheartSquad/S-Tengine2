#pragma once

#include "shader/Box2DDebugShader.h"
#include "shader/ShaderVariables.h"

Box2DDebugShader::Box2DDebugShader(bool _autoRelease):
	Shader(getVertString(), getFragString(), _autoRelease)
{
}

Box2DDebugShader::~Box2DDebugShader(){
}

std::string Box2DDebugShader::getVertString(){
	return  "#version 150" + ENDL + 
			"in vec2 position" + SEMI_ENDL +
			"void main()" + 
			"{" +
				"gl_Position = vec4(position, 0.0, 1.0);" +
			"}";
}

std::string Box2DDebugShader::getFragString(){
	return  "#version 150" + ENDL + 
			"out vec4 outColor" + SEMI_ENDL +
			"void main()" + ENDL +
			"{" + ENDL + 
			   "outColor = vec4(1.0, 1.0, 1.0, 1.0);" + ENDL +
			"}";
}