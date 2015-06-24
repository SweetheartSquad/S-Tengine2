#pragma once

#include <Transform.h>
#include <Entity.h>
#include <Box.h>
#include <node\NodeShadable.h>

class Shader;
class MeshInterface;

/** A basic entity node. Stores references to a mesh, transform, shader, parent, and list of references to children */
class MeshEntity : public virtual Entity, public virtual NodeShadable{
public:

	/** Reference to this entity's mesh */
	MeshInterface * mesh;

	// returns a box which covers the verts of the mesh and all of its children
	vox::Box calcOverallBoundingBox();

	explicit MeshEntity(MeshInterface * _mesh, Shader * _shader = nullptr);
	virtual ~MeshEntity(void);

	/**
	* Pushes model matrix stack,
	* Applies the model matrix of transform,
	* Loads and cleans mesh (if necessary),
	* Renders mesh,
	* Calls render on children (recursive),
	* Pops model matrix stack
	*/
	virtual void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;

	/**Sets shader to _shader*/
	virtual void setShader(Shader* _shader, bool _configureDefaultAttributes);
	/**Get shader*/
	Shader* getShader();
	/** Recursivley sets the shader to _shader for _entity's children recursivley*/
	void setShaderOnChildren(Shader * _shader);
	/** Calls unload on all children and on mesh */
	virtual void unload() override;
	/** Loads and cleans mesh, configures default vertex attributes, deletes and reloads shader, and calls unload on all children (recursive) */
	virtual void load() override;

	// bakes the transformation matrices into the mesh's verts and resets the transform
	//void freezeTransformation();
private:
	
};
