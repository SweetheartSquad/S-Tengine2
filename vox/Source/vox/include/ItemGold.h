#pragma once

#include <ItemSimpleWeapon.h>

class ItemGold : public ItemSimpleWeapon{
public:
	static MeshInterface * goldMesh;

	explicit ItemGold (Box2DWorld * _world);
	void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOption) override;
	void update(Step * _step) override;
};