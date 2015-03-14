#pragma once 

#include <shader\ShaderComponent.h>

class HsvShaderComponent : public ShaderComponent{
public:

	float hue, saturation, value, alpha;

	// all values are given as 0 - 1, h += hue, s *= sat, v *= val, a *= alpha
	HsvShaderComponent(float _hue, float _saturation, float _value, float _alpha = 1.f);
	~HsvShaderComponent();
	
	std::string getVertexVariablesString() override;
	std::string getFragmentVariablesString() override;
	std::string getVertexBodyString() override;
	std::string getFragmentBodyString() override;
	std::string getOutColorMod() override;
	void configureUniforms(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) override;
};