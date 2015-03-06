#pragma once

#include "Boulder.h"
#include <Box2DSprite.h>
#include <Texture.h>
#include "Box2DWorld.h"

Boulder::Boulder(Box2DWorld* _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex):
	Item(true, _world, _categoryBits, _maskBits, _groupIndex, 10.f, 0, 36),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	NodeRenderable()
{
	componentScale = 0.008f;

	TextureSampler boulderTex = TextureSampler(new Texture("../assets/structure components/catapult/Boulder1.png", 512, 512, true, true), 108, 103);
	
	boulder = new Box2DSprite(_world, b2_dynamicBody, false, nullptr, new Transform(), boulderTex.width, boulderTex.height, boulderTex.texture, componentScale);
	
	components.push_back(&boulder);
	
	b2Filter sf;
	sf.categoryBits = categoryBits;
	if(maskBits != (int16)-1){
		sf.maskBits = maskBits;
	}else{
		sf.maskBits = 0;
	}
	sf.groupIndex = groupIndex;
	
	for(Box2DSprite ** c : components){
		(*c)->createFixture(sf);
	}

	setUserData(this);
}

Boulder::~Boulder(){
}

void Boulder::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack){
	Item::render(_matrixStack, _renderStack);
}

void Boulder::update(Step* _step){
	Item::update(_step);
}

void Boulder::unload(){
	Item::unload();
}

void Boulder::load(){
	Item::load();
}