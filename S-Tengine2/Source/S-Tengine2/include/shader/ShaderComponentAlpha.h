#pragma once 

#include <shader\ShaderComponent.h>

class ShaderComponentAlpha : public ShaderComponent{
private:
	float alpha;
	GLint alphaLoc;
public:
	float getAlpha();
	void setAlpha(float _alpha);

	// all values are given as 0 - 1, h += hue, s *= sat, v *= val, a *= alpha
	ShaderComponentAlpha(Shader * _shader, float _alpha = 1.f);
	~ShaderComponentAlpha();
	
	std::string getVertexVariablesString() override;
	std::string getFragmentVariablesString() override;
	std::string getVertexBodyString() override;
	std::string getFragmentBodyString() override;
	std::string getOutColorMod() override;
	void load() override;
	void configureUniforms(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) override;
};