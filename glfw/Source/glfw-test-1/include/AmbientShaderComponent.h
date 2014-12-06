#pragma once

#include "ShaderComponent.h"

class AmbientShaderComponent : public ShaderComponent{
public:
	AmbientShaderComponent();
	~AmbientShaderComponent() override;
	std::string getVertexVariablesString() override;
	std::string getFragmentVariablesString() override;
	std::string getVertexBodyString() override;
	std::string getFragmentBodyString() override;
	std::string getOutColorMod() override;
};