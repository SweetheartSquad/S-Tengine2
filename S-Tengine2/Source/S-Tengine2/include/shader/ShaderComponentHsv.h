#pragma once 

#include <shader\ShaderComponent.h>

class ShaderComponentHsv : public ShaderComponent{
private:
	float hue, saturation, value;
	GLint hueLoc, satLoc, valLoc;
public:
	float getHue();
	float getSaturation();
	float getValue();
	void setHue(float _hue);
	void setSaturation(float _sat);
	void setValue(float _val);

	// all values are given as 0 - 1, h += hue, s *= sat, v *= val
	ShaderComponentHsv(Shader * _shader, float _hue, float _saturation, float _value);
	~ShaderComponentHsv();
	
	std::string getVertexVariablesString() override;
	std::string getFragmentVariablesString() override;
	std::string getVertexBodyString() override;
	std::string getFragmentBodyString() override;
	std::string getOutColorMod() override;
	void load() override;
	void configureUniforms(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) override;
};