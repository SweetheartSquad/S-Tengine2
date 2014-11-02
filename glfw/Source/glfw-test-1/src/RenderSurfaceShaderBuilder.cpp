#include "RenderSurfaceShaderBuilder.h"

RenderSurfaceShaderBuilder::RenderSurfaceShaderBuilder(){
	vertexShader = "#version 150\n"
	"#extension GL_ARB_explicit_attrib_location : enable\n"
	"layout(location = 1) in vec2 aVertexPosition;"
	"layout(location = 2) in vec2 aVertexUVs;"
	"out vec2 Texcoord;"
	"void main() {"
		"Texcoord = aVertexUVs;"
		"gl_Position = vec4(aVertexPosition, 0.0, 1.0);"
	"}";
}

RenderSurfaceShaderBuilder::~RenderSurfaceShaderBuilder(){
}

void RenderSurfaceShaderBuilder::pushFilter(RenderSurfaceFilter* _filter){
}

void RenderSurfaceShaderBuilder::buildFragmentShader(){
}