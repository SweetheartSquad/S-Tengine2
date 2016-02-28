#pragma once

#include <Transform.h>
#include <Entity.h>
#include <Box.h>
#include <node\NodeShadable.h>

class Shader;
class MeshInterface;

/** A basic entity node. Stores references to a mesh, transform, shader, parent, and list of references to children */
class MeshEntity : public virtual Entity, public NodeShadable{
public:

	/** Reference to this entity's mesh */
	MeshInterface * mesh;
	// Reference to the transform which is a child of this entity's childTransform and the parent of its mesh
	Transform * meshTransform;

	// returns a box which covers the verts of the mesh and all of its children
	//sweet::Box calcOverallBoundingBox();

	explicit MeshEntity(MeshInterface * _mesh, Shader * _shader = nullptr, bool _configureDefaultVertexAttributes = true);
	virtual ~MeshEntity(void);

	/**
	* Pushes model matrix stack,
	* Applies the model matrix of transform,
	* Loads and cleans mesh (if necessary),
	* Renders mesh,
	* Calls render on children (recursive),
	* Pops model matrix stack
	*/
	virtual void render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;

	/**Sets shader to _shader*/
	virtual void setShader(Shader* _shader, bool _configureDefaultAttributes = true);
	/**Get shader*/
	Shader* getShader() const;
	/** Recursivley sets the shader to _shader for _entity's children recursivley*/
	void setShaderOnChildren(Shader * _shader);
	/** Calls unload on all children and on mesh */
	virtual void unload() override;
	/** Loads and cleans mesh, configures default vertex attributes, deletes and reloads shader, and calls unload on all children (recursive) */
	virtual void load() override;

	// bakes the childTransform and meshTransform into the mesh's verts and resets them both to identity matrices
	// this is useful for static entities, as it simplifies model-matrix calculations
	// this is also useful for configuring physics entities which use the mesh as the source for the collider
	// be careful about using this with non-static entities
	void freezeTransformation();
private:
	
};
