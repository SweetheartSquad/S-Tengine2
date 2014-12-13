#pragma once

#include "Transform.h"
#include "Vertex.h"
#include "Node.h"
#include "NodeHierarchical.h"
#include "Vox.h"
#include "NodeAnimatable.h"
#include "NodeRenderable.h"
#include "NodeLoadable.h"

/** A basic entity node. Stores references to a mesh, transform, shader, parent, and list of references to children */
class Entity : public NodeAnimatable, public NodeHierarchical, public NodeRenderable, public NodeLoadable{
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
	virtual void unload();
	/** Calls unload on all children */
	virtual void load();
};
