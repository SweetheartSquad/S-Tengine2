#pragma once
#include <vector>
#include <string>
#include <RenderSurfaceFilter.h>

class RenderSurfaceShaderBuilder{
private:
	std::vector<RenderSurfaceFilter *> filters;
	void buildFragmentShader();

public:
	RenderSurfaceShaderBuilder();
	~RenderSurfaceShaderBuilder();

	std::string vertexShader;
	std::string fragmentShader;

	void pushFilter(RenderSurfaceFilter * _filter);
};