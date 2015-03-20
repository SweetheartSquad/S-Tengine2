#pragma once

#include <ItemFireballLauncher.h>
#include <ItemFireball.h>
#include <PuppetScene.h>
#include <shader\BaseComponentShader.h>

#include <SlayTheDragonResourceManager.h>
#include <PuppetResourceManager.h>

ItemFireballLauncher::ItemFireballLauncher(PuppetCharacterDragon * _dragon, Box2DWorld * _world) :
	ItemProjectileWeapon(SlayTheDragonResourceManager::itemFireball, PuppetResourceManager::itemNone, _world, PuppetGame::kITEM),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	dragon(_dragon)
{
}

Item * ItemFireballLauncher::getProjectile(){
	//Item * projectile = new Item(true, projectileTemplate->world, projectileTemplate->categoryBits, projectileTemplate->maskBits, projectileTemplate->groupIndex, projectileTemplate->damage, projectileTemplate->handleX, projectileTemplate->handleY);
	ItemFireball * projectile = new ItemFireball(dragon, world);

	Box2DSprite ** test = new Box2DSprite*[1];
	test[0] = projectile->rootComponent = new Box2DSprite(world, b2_dynamicBody, false, nullptr, new Transform(), projectileTex->width, projectileTex->height, projectileTex->texture, componentScale);

	projectile->components.push_back(test);

	b2Filter sf;
	sf.categoryBits = projectile->categoryBits;
	if (projectile->maskBits != (int16)-1){
		sf.maskBits = projectile->maskBits;
	}
	else{
		sf.maskBits = 0;
	}
	sf.groupIndex = projectile->groupIndex;

	for (Box2DSprite ** c : projectile->components){
		(*c)->createFixture(sf);
		(*c)->body->GetFixtureList()->SetDensity(0.01f);
		(*c)->body->ResetMassData();
	}

	projectile->setUserData(projectile);

	static_cast<PuppetScene *>(scene)->addChild(projectile, 1);
	static_cast<PuppetScene *>(scene)->items.push_back(projectile);
	//projectile->addToLayeredScene(static_cast<PuppetScene *>(scene), 1);
	projectile->setShader(static_cast<PuppetScene *>(scene)->shader, true);

	projectile->snapComponents(this->rootComponent);
	//projectile->translateComponents(glm::vec3(20,20,0));

	projectile->thrown = true;
	projectile->held = false;

	return projectile;
}