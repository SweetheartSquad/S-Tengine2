#pragma once

#include <node\Node.h>

class Shader;
class RenderOptions;
class NodeShadable abstract : public virtual Node{
public:
	Shader * shader;

	NodeShadable(Shader * _shader = nullptr);
	virtual ~NodeShadable();

	// if no override shader is present on _renderOptions, applies this node's shader to the shader option
	virtual void applyShader(RenderOptions * _renderOptions);
};