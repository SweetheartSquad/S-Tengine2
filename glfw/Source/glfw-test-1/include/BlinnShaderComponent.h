#pragma once 

#include "ShaderComponent.h"

class BlinnShaderComponent : public ShaderComponent{
public:
	BlinnShaderComponent();
	~BlinnShaderComponent() override;
	std::string getVertexVariablesString() override;
	std::string getFragmentVariablesString() override;
	std::string getVertexBodyString() override;
	std::string getFragmentBodyString() override;
	std::string getOutColorMod() override;
};