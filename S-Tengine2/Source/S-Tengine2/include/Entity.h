#pragma once

#include "Transform.h"
#include "Vertex.h"
#include "node/Node.h"
#include "Vox.h"
#include "node/NodeUpdatable.h"
#include "node/NodeRenderable.h"
#include "node/NodeLoadable.h"

/** A basic entity node. Stores references to a mesh, transform, shader, parent, and list of references to children */
class Entity : public virtual NodeUpdatable, public virtual NodeRenderable, public virtual NodeLoadable, public virtual NodeChild{
public:
	explicit Entity();
	virtual ~Entity(void);
	void makeCumulativeModelMatrixDirty() override;

	/**
	* Pushes model matrix stack,
	* Applies the model matrix of transform,
	* Loads and cleans mesh (if necessary),
	* Renders mesh,
	* Calls render on children (recursive),
	* Pops model matrix stack
	*/
	virtual void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;
	/** Doesn't do anything by default */
	virtual void update(Step * _step) override;

	/** Calls unload on all children */
	virtual void unload() override;
	/** Calls unload on all children */
	virtual void load() override;

	// constant reference to a transform which acts as a container for children of this entity
	Transform * const childTransform;
	virtual void addParent(Transform * _parent) override;
	virtual void removeParent(Transform * _parent) override;


	// prints the hierarchy to the console in ASCII
	virtual void printHierarchy(unsigned long int _startDepth = 0) override;

	void deleteChildTransform();
private:
	bool childTransformExists;
};