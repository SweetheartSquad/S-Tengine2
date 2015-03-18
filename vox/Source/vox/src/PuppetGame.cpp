#include "PuppetGame.h"
#include "RaidTheCastle.h"
#include <Rapunzel.h>

#include "GameJamCharacter.h"
#include "GameJamSceneIndoor.h"
#include "GameJamSceneOutdoor.h"

#include <cstdlib>
#include <ctime>
#include <BaseScene.h>
#include <AccelerometerParser.h>
#include <Accelerometer.h>
#include <PuppetController.h>

PuppetGame::PuppetGame(bool _running):
	Game(_running),
	arduino(new AccelerometerParser("COM4"))
{
	std::srand((unsigned long int)std::time(0));
	//GameJamCharacter::init();
	PuppetResourceManager::init();
	PuppetResourceManager::load();

	//Arduino stuff must be setup before the first scene is intiailized
	Accelerometer * acc = new Accelerometer(arduino);
	arduino->addAccelerometer(acc);

	Accelerometer * acc2 = new Accelerometer(arduino);
	arduino->addAccelerometer(acc2);

	Accelerometer * acc3 = new Accelerometer(arduino);
	arduino->addAccelerometer(acc3);
	
	Accelerometer * acc4 = new Accelerometer(arduino);
	arduino->addAccelerometer(acc4);
	
	puppetController1 = new PuppetController(acc,  nullptr);
	puppetController2 = new PuppetController(acc2, nullptr);
	puppetController3 = new PuppetController(acc3, nullptr);
	puppetController4 = new PuppetController(acc4, nullptr);

	//scenes.insert(std::make_pair("indoors", new GameJamSceneIndoor(this)));
	//scenes.insert(std::make_pair("outdoors", new GameJamSceneOutdoor(this)));
	scenes.insert(std::make_pair("Raid the Castle", new Rapunzel(this)));
	//((GameJamSceneIndoor *)scenes.at("Raid the Castle"))->debugDraw = true;
	//currentScene = scenes.at("indoors");
	currentSceneKey = "Raid the Castle";
	currentScene = scenes.at(currentSceneKey);
}

PuppetGame::~PuppetGame(){
}

void PuppetGame::update(){
    arduino->update(&vox::step);
    puppetController1->update(&vox::step);
    puppetController2->update(&vox::step);
    puppetController3->update(&vox::step);
    puppetController4->update(&vox::step);

	Game::update();
}

void PuppetGame::draw(){
	Game::draw();
}