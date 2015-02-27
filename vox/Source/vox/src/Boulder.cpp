#pragma once

#include "Boulder.h"
#include <Box2DSprite.h>
#include <Texture.h>
#include "Box2DWorld.h"

Boulder::Boulder(Box2DWorld* _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex):
	Item(_world, _categoryBits, _maskBits, _groupIndex, 10.f),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	NodeRenderable()
{
	componentScale = 0.008f;

	TextureSampler boulderTex = TextureSampler(new Texture("../assets/structure components/catapult/Boulder1.png", 512, 512, true, true), 108, 103);
	
	boulder = new Box2DSprite(_world, b2_dynamicBody, false, nullptr, new Transform(), boulderTex.width, boulderTex.height, boulderTex.texture, componentScale);
	
	components.push_back(&boulder);
	
	for(Box2DSprite ** c : components){
		(*c)->createFixture(groupIndex);
	}
	
	b2PolygonShape tShape;
	tShape.SetAsBox(boulder->width*std::abs(transform->scaleVector.x)*boulder->scale*2.f, std::abs(boulder->height*transform->scaleVector.y)*boulder->scale*2.f);
	
	b2Fixture * sensor = boulder->body->CreateFixture(&tShape, 1);
	sensor->SetSensor(true);
	sensor->SetUserData(this);
	sensor->SetDensity(10.f);
	
	b2Filter sf;
	sf.categoryBits = categoryBits;
	if(maskBits != (int16)-1){
		sf.maskBits = maskBits;
	}
	sf.groupIndex = groupIndex;
	sensor->SetFilterData(sf);
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