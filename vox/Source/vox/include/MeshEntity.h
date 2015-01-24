#pragma once

#include "Entity.h"
#include "Vox.h"
#include "Transform.h"

class Shader;
class MeshInterface;

/** A basic entity node. Stores references to a mesh, transform, shader, parent, and list of references to children */
class MeshEntity : public Entity{
public:

	/** Reference to this entity's mesh */
	MeshInterface * mesh;

	explicit MeshEntity(MeshInterface * _mesh = nullptr, Transform * _transform = new Transform(), Shader * _shader = nullptr);
	virtual ~MeshEntity(void);

	/**
	* Pushes model matrix stack,
	* Applies the model matrix of transform,
	* Loads and cleans mesh (if necessary),
	* Renders mesh,
	* Calls render on children (recursive),
	* Pops model matrix stack
	*/
	virtual void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack) override;
	/** Doesn't do anything by default */
	virtual void update(Step * _step) override;

	virtual void removeChildAtIndex(int _index) override;
	/**Sets shader to _shader*/
	void setShader(Shader* _shader, bool _configureDefaultAttributes);
	/**Get shader*/
	Shader* getShader();
	/** Recursivley sets the shader to _shader for _entity's children recursivley*/
	void setShaderOnChildren(Shader * _shader);
	/** Calls unload on all children and on mesh */
	virtual void unload() override;
	/** Loads and cleans mesh, configures default vertex attributes, deletes and reloads shader, and calls unload on all children (recursive) */
	virtual void load() override;


private:
	
	/** Reference to this entity's shader */
	Shader * shader;
};
