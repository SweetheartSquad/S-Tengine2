#pragma once

#include "ShaderComponent.h"

class PhongShaderComponent : public ShaderComponent{
public:
	PhongShaderComponent();
	~PhongShaderComponent() override;
	std::string getVertexVariablesString() override;
	std::string getFragmentVariablesString() override;
	std::string getVertexBodyString() override;
	std::string getFragmentBodyString() override;
	std::string getOutColorMod() override;
};