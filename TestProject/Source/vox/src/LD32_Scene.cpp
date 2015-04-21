#pragma once

#include <LD32_Scene.h>
#include <LD32_EndScene.h>
#include <LD32_Game.h>
#include <LD32_ResourceManager.h>
#include <LD32_Donut.h>
#include <LD32_Player.h>
#include <LD32_Enemy.h>
#include <LD32_ContactListener.h>

#include <MeshEntity.h>
#include <MeshInterface.h>
#include <MeshFactory.h>
#include <Resource.h>

#include <DirectionalLight.h>
#include <PointLight.h>
#include <Material.h>

#include <shader\BaseComponentShader.h>
#include <shader\ShaderComponentTexture.h>
#include <shader\ShaderComponentDiffuse.h>
#include <shader\ShaderComponentPhong.h>
#include <shader\ShaderComponentBlinn.h>
#include <shader\ShaderComponentShadow.h>
#include <shader\ShaderComponentHsv.h>

#include <Box2DWorld.h>
#include <Box2DMeshEntity.h>
#include <Box2DDebugDraw.h>

#include <MousePerspectiveCamera.h>
#include <FollowCamera.h>

#include <Sound.h>
#include <libzplay.h>

#include <Keyboard.h>
#include <GLFW\glfw3.h>
#include <MatrixStack.h>

#include <RenderSurface.h>
#include <StandardFrameBuffer.h>
#include <NumberUtils.h>
#include <LD32_Monster.h>

LD32_Scene::LD32_Scene(Game * _game) :
	Scene(_game),
	shader(new BaseComponentShader(true)),
	world(new Box2DWorld(b2Vec2(0, -10))),
	drawer(nullptr),
	player(nullptr),
	sceneHeight(150),
	sceneWidth(50),
	screenSurfaceShader(new Shader("../assets/RenderSurface", false, true)),
	screenSurface(new RenderSurface(screenSurfaceShader)),
	screenFBO(new StandardFrameBuffer(true)),
	phongMat(new Material(15.0, glm::vec3(1.f, 1.f, 1.f), true)),
	hsvComponent(new ShaderComponentHsv(shader, 0, 0, 0))
{
	shader->components.push_back(new ShaderComponentTexture(shader));
	shader->components.push_back(new ShaderComponentDiffuse(shader));
	shader->components.push_back(hsvComponent);
	//shader->components.push_back(new ShaderComponentPhong(shader));
	//shader->components.push_back(new ShaderComponentBlinn(shader));
	//shader->components.push_back(new ShaderComponentShadow(shader));
	shader->compileShader();

	//Set up cameras
	mouseCam = new MousePerspectiveCamera();
	cameras.push_back(mouseCam);
	mouseCam->farClip = 1000.f;
	mouseCam->nearClip = 0.001f;
	mouseCam->transform->rotate(90, 0, 1, 0, kWORLD);
	mouseCam->transform->translate(5.0f, 1.5f, 22.5f);
	mouseCam->yaw = 90.0f;
	mouseCam->pitch = -10.0f;
	mouseCam->speed = 1;

	debugCam = new MousePerspectiveCamera();
	cameras.push_back(debugCam);
	debugCam->farClip = 1000.f;
	debugCam->transform->rotate(90, 0, 1, 0, kWORLD);
	debugCam->transform->translate(5.0f, 1.5f, 22.5f);
	debugCam->yaw = 90.0f;
	debugCam->pitch = -10.0f;
	debugCam->speed = 1;

	gameCam = new FollowCamera(15, glm::vec3(0, 0, 0), 0, 0);
	cameras.push_back(gameCam);
	gameCam->farClip = 1000.f;
	gameCam->transform->rotate(90, 0, 1, 0, kWORLD);
	gameCam->transform->translate(5.0f, 1.5f, 22.5f);
	gameCam->minimumZoom = 22.5f;
	gameCam->yaw = 90.0f;
	gameCam->pitch = -10.0f;
	activeCamera = gameCam;
	
	float _size = 3;
	std::vector<Box2DMeshEntity *> boundaries;
	MeshInterface * boundaryMesh = MeshFactory::getPlaneMesh();
	boundaries.push_back(new Box2DMeshEntity(world, boundaryMesh, b2_staticBody));
	boundaries.push_back(new Box2DMeshEntity(world, boundaryMesh, b2_staticBody));
	boundaries.push_back(new Box2DMeshEntity(world, boundaryMesh, b2_staticBody));
	boundaries.push_back(new Box2DMeshEntity(world, boundaryMesh, b2_staticBody));

	boundaries.at(0)->transform->scale(_size, sceneHeight*0.5f + _size*2.f, _size);
	boundaries.at(1)->transform->scale(_size, sceneHeight*0.5f + _size*2.f, _size);
	boundaries.at(2)->transform->scale(sceneWidth*0.5f + _size*2.f, _size, _size);
	boundaries.at(3)->transform->scale(sceneWidth*0.5f + _size*2.f, _size, _size);

	boundaries.at(0)->setTranslationPhysical(sceneWidth+_size, sceneHeight*0.5f, 0);
	boundaries.at(1)->setTranslationPhysical(-_size, sceneHeight*0.5f, 0);
	boundaries.at(2)->setTranslationPhysical(sceneWidth*0.5f, sceneHeight+_size, 0);
	boundaries.at(3)->setTranslationPhysical(sceneWidth*0.5f, -_size, 0);
	
	b2Filter sf;
	//sf.categoryBits = PuppetGame::kBOUNDARY;
	//sf.maskBits = -1;
	for(auto b : boundaries){
		addChild(b);
		b->setShader(shader, true);
		world->addToWorld(b);
		b->body->GetFixtureList()->SetFilterData(sf);
		//b->mesh->pushTexture2D(PuppetResourceManager::stageFront);
	}
	//sf.categoryBits = PuppetGame::kBOUNDARY | PuppetGame::kGROUND;
	boundaries.at(3)->body->GetFixtureList()->SetFilterData(sf);
	boundaries.at(3)->body->GetFixtureList()->SetFriction(1);
	boundaries.at(3)->body->GetFixtureList()->SetRestitution(0);

	MeshEntity * ground = new MeshEntity(MeshFactory::getPlaneMesh());
	ground->transform->translate(sceneWidth/2.f, sceneHeight/2.f, -0.5f);
	ground->transform->scale(sceneWidth, sceneHeight, 1);
	ground->setShader(shader, true);
	addChild(ground);


	//lights.push_back(new DirectionalLight(glm::vec3(1,0,0), glm::vec3(1,1,1), 1));
	
	player = new LD32_Player(world);
	player->setShader(shader, true);
	gameCam->addTarget(player, 1);
	addChild(player);
	player->setTranslationPhysical(sceneWidth / 2.f, sceneHeight / 8.f, 0, false);
	for(auto i : player->things){
		i->setTranslationPhysical(sceneWidth / 2.f, sceneHeight / 8.f, 0, false);
	}

	Sound::masterVolume = 0;
	LD32_ResourceManager::music->play("bgm", true);
	LD32_ResourceManager::music->play("bgm2", true);
	LD32_ResourceManager::music->play("bgm3", true);
	LD32_ResourceManager::music->play("bgm4", true);
	Sound::masterVolume = 100;
	/*for(auto m : LD32_ResourceManager::music->sounds){
		m.second.player->SetPlayerVolume(0, 0);
	}*/
	
	MeshInterface * avMesh = MeshFactory::getPlaneMesh();
	for(Vertex & v : avMesh->vertices){
		if(v.y < 0.f){
			v.y += 1.f;
		}
		v.red = 1;
		v.green = 0;
		v.blue = v.y;
		v.alpha = 1-v.y;
	}
	avMesh->dirty = true;
	for(int i = 0; i < numHarmonics; ++i){
		MeshEntity * m = new MeshEntity(avMesh);
		
		m->transform->translate(sceneWidth/numHarmonics/2.f * (i+0.5f), 0, 1);
		m->setShader(shader, true);
		addChild(m);
		audioVisualizer.push_back(m);
	}
	for(int i = 0; i < numHarmonics; ++i){
		MeshEntity * m = new MeshEntity(avMesh);

		m->transform->translate(sceneWidth - (sceneWidth/numHarmonics)/2.f * (i+0.5f), 0, 1);
		m->setShader(shader, true);
		addChild(m);
		audioVisualizer.push_back(m);
	}

	for(float i = 1; i < sceneHeight-10; i += 6){
		{LD32_Donut * donut = new LD32_Donut(world);
		donut->setTranslationPhysical(sceneWidth/2.f + (i+3)/sceneHeight*sceneWidth/2.f + (fmod(i+13, 15)+1), i+1, 0);
		donut->setShaderOnChildren(shader);
		addChild(donut);}

		{LD32_Donut * donut = new LD32_Donut(world);
		donut->setTranslationPhysical(sceneWidth/2.f - (i+3)/sceneHeight*sceneWidth/2.f - (fmod(i+13, 15)+1), i+1, 0);
		donut->setShaderOnChildren(shader);
		addChild(donut);}
	}

	
	//intialize key light
	PointLight * keyLight = new PointLight(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(1.f, 1.f, 1.f), 0.00f, -0.01f, -10.f);
	//Set it as the key light so it casts shadows
	//keyLight->isKeyLight = true;
	//Add it to the scene
	lights.push_back(keyLight);
	player->addChild(keyLight);
	
	mouseCam->upVectorLocal = glm::vec3(0, 0, 1);
	mouseCam->forwardVectorLocal = glm::vec3(1, 0, 0);
	mouseCam->rightVectorLocal = glm::vec3(0, -1, 0);

	LD32_ContactListener * cl = new LD32_ContactListener(this);
	world->b2world->SetContactListener(cl);

	monster = new LD32_Monster(world);
	monster->setShader(shader, true);
	monster->transform->rotate(90, 1.f, 0.f, 0.f, kOBJECT);
	monster->setTranslationPhysical(sceneWidth * 0.5, sceneHeight * 0.5, 1.f, false);
	monster->transform->scale(2.0f, 2.0f, 2.0f, true);
	addChild(monster);
}

LD32_Scene::~LD32_Scene(){
	while(children.size() > 0){
		NodeHierarchical::deleteRecursively(children.back());
		children.pop_back();
	}
	
	shader->safeDelete();
	//delete phongMat;
	delete world;

	delete screenSurface;
	//screenSurfaceShader->safeDelete();
	screenFBO->safeDelete();
}

void LD32_Scene::update(Step * _step){
	/*clearColor[0] = 0.1f;
	clearColor[1] = 0.1f;
	clearColor[2] = 0.1f;*/

	int fftDataL[numHarmonics];
	int fftDataR[numHarmonics];

	LD32_ResourceManager::music->sounds.at("bgm").player->GetFFTData(numFFTsamples, libZPlay::TFFTWindow::fwBlackmanNuttall, nullptr, nullptr, fftDataL, fftDataR, nullptr, nullptr);
	
	lights.at(0)->data.intensities = glm::vec3(fftDataL[1]/200.f, fftDataL[2]/200.f, fftDataR[1]/200.f) + glm::vec3(0.5f, 0.5f, 0.5f);
	//lights.at(0)->data.cutoff = fftDataL[2]/5.f;
	lights.at(0)->data.attenuation = (-0.005f + fftDataL[1] / 50000.f) * (player->hits+1);
	//lights.at(0)->transform->translate(-mouseCam->forwardVectorRotated, false);
	
	hsvComponent->setValue((fftDataL[1] + fftDataR[1]) / 100.f);
	hsvComponent->setSaturation((fftDataL[numHarmonics-1] + fftDataR[numHarmonics-1]) / 200.f + 0.25f);
	hsvComponent->setHue(hsvComponent->getHue() + (fftDataL[4] + fftDataR[4]) / 1000.f);

	for(int i = 0; i < numHarmonics; ++i){
		audioVisualizer.at(i)->transform->scale(sceneWidth / numHarmonics / 2.f, (fftDataL[i]) / 10.f, 1, false);
	}for(int i = 0; i < numHarmonics; ++i){
		audioVisualizer.at(i+numHarmonics)->transform->scale(sceneWidth / numHarmonics / 2.f, (fftDataR[i]) / 10.f, 1, false);
	}
	
	//shader->components.at(0)->makeDirty();
	//shader->components.at(1)->makeDirty();

	if(keyboard->keyJustUp(GLFW_KEY_F11)){
		game->toggleFullScreen();
	}

	// camera controls
	if (keyboard->keyDown(GLFW_KEY_UP)){
		activeCamera->transform->translate((activeCamera->forwardVectorRotated) * static_cast<MousePerspectiveCamera *>(activeCamera)->speed);
	}
	if (keyboard->keyDown(GLFW_KEY_DOWN)){
		activeCamera->transform->translate((activeCamera->forwardVectorRotated) * -static_cast<MousePerspectiveCamera *>(activeCamera)->speed);
	}
	if (keyboard->keyDown(GLFW_KEY_LEFT)){
		activeCamera->transform->translate((activeCamera->rightVectorRotated) * -static_cast<MousePerspectiveCamera *>(activeCamera)->speed);
	}
	if (keyboard->keyDown(GLFW_KEY_RIGHT)){
		activeCamera->transform->translate((activeCamera->rightVectorRotated) * static_cast<MousePerspectiveCamera *>(activeCamera)->speed);
	}
	
	// player controls
	if(player != nullptr){
		float playerSpeed = 0.1f;
		float playerRotSpeed = 0.5f;
		float mass = player->body->GetMass();
		float angle = atan2(mouseCam->forwardVectorRotated.y, mouseCam->forwardVectorRotated.x);

		if(activeCamera != mouseCam){
			angle = glm::radians(90.f);
		}

		mouseCam->transform->translate(player->getPos(false) + glm::vec3(0, 0, player->transform->getScaleVector().z*1.25f), false);
		mouseCam->lookAtOffset = glm::vec3(0, 0, -player->transform->getScaleVector().z*0.25f);
		
		
		if (keyboard->keyDown(GLFW_KEY_W)){
			player->applyLinearImpulseUp(playerSpeed * mass * sin(angle));
			player->applyLinearImpulseRight(playerSpeed * mass * cos(angle));
		}
		if (keyboard->keyDown(GLFW_KEY_S)){
			player->applyLinearImpulseDown(playerSpeed * mass * sin(angle));
			player->applyLinearImpulseLeft(playerSpeed * mass * cos(angle));
		}
		unsigned int p1, p2;
		if (keyboard->keyDown(GLFW_KEY_A)){
			player->applyLinearImpulseUp(playerSpeed * mass * cos(angle));
			player->applyLinearImpulseLeft(playerSpeed * mass * sin(angle));

			player->body->ApplyAngularImpulse(playerRotSpeed * mass, true);
			LD32_ResourceManager::music->sounds.at("bgm3").player->GetPlayerVolume(&p1, &p2);
			if(p1 < 100){
				LD32_ResourceManager::music->sounds.at("bgm3").player->SetPlayerVolume(p1+1, p2+1);
			}
		}else{
			LD32_ResourceManager::music->sounds.at("bgm3").player->GetPlayerVolume(&p1, &p2);
			if(p1 > 50){
				LD32_ResourceManager::music->sounds.at("bgm3").player->SetPlayerVolume(p1-1, p2-1);
			}
		}
		if (keyboard->keyDown(GLFW_KEY_D)){
			player->applyLinearImpulseDown(playerSpeed * mass * cos(angle));
			player->applyLinearImpulseRight(playerSpeed * mass * sin(angle));

			player->body->ApplyAngularImpulse(-playerRotSpeed * mass, true);
			LD32_ResourceManager::music->sounds.at("bgm2").player->GetPlayerVolume(&p1, &p2);
			if(p1 < 100){
				LD32_ResourceManager::music->sounds.at("bgm2").player->SetPlayerVolume(p1+1, p2+1);
			}
		}else{
			LD32_ResourceManager::music->sounds.at("bgm2").player->GetPlayerVolume(&p1, &p2);
			if(p1 > 50){
				LD32_ResourceManager::music->sounds.at("bgm2").player->SetPlayerVolume(p1-1, p2-1);
			}
		}
		if (keyboard->keyJustDown(GLFW_KEY_SPACE)){
			LD32_ResourceManager::sfx->playRandomSound();
		}

		if (keyboard->keyDown(GLFW_KEY_SPACE)){
			//player->thing1->applyLinearImpulseDown(playerSpeed * mass * cos(angle));
			//player->thing1->applyLinearImpulseRight(playerSpeed * mass * sin(angle));
			for (auto j : player->joints){
				j->SetLimits(0, 1.5);
			}
			unsigned int p1, p2;
			LD32_ResourceManager::music->sounds.at("bgm").player->GetPlayerVolume(&p1, &p2);
			if(p1 < 100){
				LD32_ResourceManager::music->sounds.at("bgm").player->SetPlayerVolume(p1+1, p2+1);
			}
			LD32_ResourceManager::music->sounds.at("bgm4").player->GetPlayerVolume(&p1, &p2);
			if(p1 < 100){
				LD32_ResourceManager::music->sounds.at("bgm4").player->SetPlayerVolume(p1+1, p2+1);
			}
		}else{
			for (auto j : player->joints){
				j->SetLimits(0, 0);
			}
			LD32_ResourceManager::music->sounds.at("bgm").player->GetPlayerVolume(&p1, &p2);
			if(p1 > 75){
				LD32_ResourceManager::music->sounds.at("bgm").player->SetPlayerVolume(p1-1, p2-1);
			}else if(p1 < 75){
				LD32_ResourceManager::music->sounds.at("bgm").player->SetPlayerVolume(p1+1, p2+1);
			}
			LD32_ResourceManager::music->sounds.at("bgm4").player->GetPlayerVolume(&p1, &p2);
			if(p1 > 50){
				LD32_ResourceManager::music->sounds.at("bgm4").player->SetPlayerVolume(p1-1, p2-1);
			}else if(p1 < 50){
				LD32_ResourceManager::music->sounds.at("bgm4").player->SetPlayerVolume(p1+1, p2+1);
			}
		}

	}

	b2Vec2 dir = player->body->GetPosition() - monster->body->GetPosition();
	//monster->transform->setOrientation(glm::quat(glm::angleAxis(atan2(dir.y, dir.x), glm::vec3(0, 0, 1))));
	monster->body->SetTransform(monster->body->GetPosition(), atan2(dir.y, dir.x)); 


	if(monster->spawnEnemy > 10){
		LD32_Enemy * enemy = new LD32_Enemy(world);
		enemy->setTranslationPhysical(monster->getPos(false));
		//enemy->setTranslationPhysical(dir.x*3, dir.y*3, 0, true);
		enemy->applyLinearImpulse(dir.x*10, dir.y*10, enemy->getPos(false).x, enemy->getPos(false).y);
		enemy->setShaderOnChildren(shader); // <- this is the problem?
		addChild(enemy);
		enemies.push_back(enemy);
		LD32_ResourceManager::miscSounds->play("spawn");
		monster->spawnEnemy = 0;
	}else{
		monster->spawnEnemy += _step->deltaTime;
	}

	for(signed long int i = enemies.size()-1; i > 0; --i){
		if(enemies.at(i)->heDed){
			removeChild(enemies.at(i));
			delete enemies.at(i);
			enemies.erase(enemies.begin() + i);
		}
	}

	// debug controls
	if(keyboard->keyJustDown(GLFW_KEY_1)){
		if(activeCamera == gameCam){
			activeCamera = mouseCam;
		}else if(activeCamera == mouseCam){
			activeCamera = debugCam;
		}else{
			activeCamera = gameCam;
		}
	}
	if(keyboard->keyJustUp(GLFW_KEY_2)){
		if(drawer != nullptr){
			world->b2world->SetDebugDraw(nullptr);
			removeChild(drawer);
			delete drawer;
			drawer = nullptr;
		}else{
			drawer = new Box2DDebugDraw(world);
			world->b2world->SetDebugDraw(drawer);
			drawer->drawing = true;
			//drawer->AppendFlags(b2Draw::e_aabbBit);
			drawer->AppendFlags(b2Draw::e_shapeBit);
			drawer->AppendFlags(b2Draw::e_centerOfMassBit);
			drawer->AppendFlags(b2Draw::e_jointBit);
			//drawer->AppendFlags(b2Draw::e_pairBit);
			addChild(drawer);
		}
	}
	
	if(player->won){
		game->scenes.insert(std::pair<std::string, Scene *>("end", new LD32_EndScene(game, true)));
		game->switchScene("end", true);
	}
	if(player->deathPending){
		game->scenes.insert(std::pair<std::string, Scene *>("end", new LD32_EndScene(game, false)));
		game->switchScene("end", true);
	}

	Scene::update(_step);

	world->update(_step);

	
}

void LD32_Scene::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	screenFBO->resize(game->viewPortWidth, game->viewPortHeight);
	//Bind frameBuffer
	screenFBO->bindFrameBuffer();
	//render the scene to the buffer
	Scene::render(_matrixStack, _renderOptions);

	//Render the buffer to the render surface
	screenSurface->render(screenFBO->getTextureId());
}

void LD32_Scene::load(){
	Scene::load();	

	screenSurface->load();
	screenFBO->load();
}

void LD32_Scene::unload(){
	Scene::unload();	

	screenSurface->unload();
	screenFBO->unload();
}