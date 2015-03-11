#pragma once

#include <VictoryScene.h>

#include <PuppetController.h>

VictoryScene::VictoryScene(PuppetGame * _game) :
	PuppetScene(_game, 6000)
{
	/*puppetController =  new PuppetController(acc, playerCharacter);
	puppetController2 = new PuppetController(acc2, playerCharacter2);
	puppetController3 = new PuppetController(acc3, playerCharacter3);
	puppetController4 = new PuppetController(acc4, playerCharacter4);*/
}