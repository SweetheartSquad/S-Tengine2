#pragma once

#include <string>

class NodeRenderable;

namespace sweet{
	class MatrixStack;
}

class RenderOptions;
class Shader;

class GeometryComponent {
public:
	Shader * shader;

	GeometryComponent(Shader * _shader);
	virtual ~GeometryComponent();
	virtual std::string getGeometryShader() = 0;
	virtual std::string getVertexVariablesString() = 0;
	virtual std::string getVertexBodyString() = 0;
	virtual void configureUniforms(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOption,  NodeRenderable* _nodeRenderable) = 0;
	virtual void clean(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable);
	virtual void makeDirty();
private:
	bool dirty;
};
