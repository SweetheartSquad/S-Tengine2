#pragma once 

#include "ShaderComponent.h"

class DiffuseShaderComponent : public ShaderComponent{
public:
	DiffuseShaderComponent();
	~DiffuseShaderComponent() override;
	std::string getVertexVariablesString() override;
	std::string getFragmentVariablesString() override;
	std::string getVertexBodyString() override;
	std::string getFragmentBodyString() override;
	std::string getOutColorMod() override;
};