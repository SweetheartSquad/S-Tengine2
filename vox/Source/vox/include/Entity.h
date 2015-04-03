#pragma once

#include "Transform.h"
#include "Vertex.h"
#include "node/Node.h"
#include "node/NodeHierarchical.h"
#include "Vox.h"
#include "node/NodeUpdatable.h"
#include "node/NodeRenderable.h"
#include "node/NodeLoadable.h"
#include "node/ShiftKiddie.h"

/** A basic entity node. Stores references to a mesh, transform, shader, parent, and list of references to children */
class Entity : public virtual NodeUpdatable, public virtual NodeHierarchical, public virtual NodeRenderable, public virtual NodeLoadable, public virtual ShiftKiddie{
public:

	explicit Entity(Transform * _transform = nullptr);
	virtual ~Entity(void);

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
	/** Calls unload on all children */
	virtual void unload() override;
	/** Calls unload on all children */
	virtual void load() override;
};
