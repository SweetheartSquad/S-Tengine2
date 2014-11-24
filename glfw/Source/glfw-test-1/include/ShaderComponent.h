#pragma once

#include <string>

class ShaderComponent abstract{
public:
	ShaderComponent();
	virtual ~ShaderComponent();

	virtual std::string getVertexVariablesString() = 0;
	virtual std::string getFragmentVariablesString() = 0;
	virtual std::string getVertexBodyString() = 0;
	virtual std::string getFragmentBodyString() = 0;
	virtual std::string getOutColorMod() = 0;
};