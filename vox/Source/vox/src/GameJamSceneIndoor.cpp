#pragma once

#include "GameJamSceneIndoor.h"

#include "Texture.h"
#include "Sprite.h"
#include "shader/BaseComponentShader.h"
#include "shader/ShaderComponentTexture.h"
#include "shader/ShaderComponentShadow.h"
#include "shader/ShaderComponentPhong.h"
#include "Keyboard.h"
#include "SpriteSheet.h"
#include "Rectangle.h"
#include "SpriteSheetAnimation.h"
#include "Vox.h"
#include "SoundManager.h"
#include "Box2DSprite.h"
#include "Box2DWorld.h"
#include "ControllableOrthographicCamera.h"
#include "Resource.h"
#include "RenderOptions.h"

#include <array>
#include <libzplay.h>
#include <Box2D/Box2D.h>
#include <Box2DDebugDraw.h>
#include "Box2DMeshEntity.h"
#include "MeshFactory.h"
#include "PerspectiveCamera.h"
#include "MousePerspectiveCamera.h"
#include "BitmapFont.h"
#include "CylinderScreen.h"
#include "TestCharacter.h"
#include "CharacterComponent.h"
#include "Character1.h"
#include "Character2.h"
#include "Character3.h"
#include "Character4.h"
#include "DialogEvent.h"
#include "SayAction.h"
#include "RandomCharacter.h"
#include "Game.h"

GameJamSceneIndoor::GameJamSceneIndoor(Game * _game) :
	GameJamScene(_game)
{
	backgroundScreen->mesh->pushTexture2D(new Texture("../assets/environments/skybox.png", 4096, 4096, true, true));
	midgroundScreen->mesh->pushTexture2D(new Texture("../assets/environments/walls - HD - edited.png", 4096, 4096, true, true));
	foregroundScreen->mesh->pushTexture2D(new Texture("../assets/environments/foregroundhallway.png", 4096, 4096, true, true));

	ground->mesh->pushTexture2D(new Texture("../assets/environments/bathroomtile.png", 512, 512, true, true));
}

void GameJamSceneIndoor::update(Step * _step){
	GameJamScene::update(_step);
	if(keyboard->keyJustDown(GLFW_KEY_SPACE)/* || fmod(_step->time, 100) > 99*/){
		game->switchScene("outdoors", false);
	}
}