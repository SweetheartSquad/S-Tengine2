#pragma once 

#include "ShaderComponent.h"

class LightShaderComponent : public ShaderComponent{
public:
	LightShaderComponent();
	~LightShaderComponent() override;
	std::string getVertexVariablesString() override;
	std::string getFragmentVariablesString() override;
	std::string getVertexBodyString() override;
	std::string getFragmentBodyString() override;
	std::string getOutColorMod() override;
};