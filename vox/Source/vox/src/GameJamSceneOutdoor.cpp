#pragma once

#include "GameJamSceneOutdoor.h"

#include "Texture.h"
#include "Sprite.h"
#include "shader/BaseComponentShader.h"
#include "shader/TextureShaderComponent.h"
#include "shader/ShadowShaderComponent.h"
#include "shader/PhongShaderComponent.h"
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

GameJamSceneOutdoor::GameJamSceneOutdoor(Game * _game):
	GameJamScene(_game)
{
	backgroundScreen->mesh->pushTexture2D(new Texture("../assets/environments/skybox.png", 4096, 4096, true, true));
	midgroundScreen->mesh->pushTexture2D(new Texture("../assets/environments/courtyard.png", 4096, 4096, true, true));
	foregroundScreen->mesh->pushTexture2D(new Texture("../assets/environments/foregroundhallway.png", 4096, 4096, true, true));

	ground->mesh->pushTexture2D(new Texture("../assets/environments/courtyardtile.png", 512, 512, true, true));
}


void GameJamSceneOutdoor::update(Step * _step){
	GameJamScene::update(_step);
	if(keyboard->keyJustUp(GLFW_KEY_SPACE) || fmod(_step->time, 600) > 599){
		if(game->scenes.count("indoors") != 0){
			game->currentScene = game->scenes.at("indoors");
		}
	}
}
