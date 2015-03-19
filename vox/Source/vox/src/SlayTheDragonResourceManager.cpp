#pragma once

#include <SlayTheDragonResourceManager.h>

TextureSampler * SlayTheDragonResourceManager::splashMessage	= new TextureSampler(new Texture("../assets/hurly-burly/SplashMessages/SlayTheDragon.png", 1024, 1024, true, true), 1024, 1024);

TextureSampler * SlayTheDragonResourceManager::fortBackground		= new TextureSampler(new Texture("../assets/hurly-burly/Fortification/FortBackground.png", 1024, 1024, true, true), 453, 307);
TextureSampler * SlayTheDragonResourceManager::fortForeground		= new TextureSampler(new Texture("../assets/hurly-burly/Fortification/FortForeground.png", 1024, 1024, true, true), 1024, 413);
TextureSampler * SlayTheDragonResourceManager::fortStructure		= new TextureSampler(new Texture("../assets/hurly-burly/Fortification/FortStructure.png", 1024, 1024, true, true), 683, 127);
Texture * SlayTheDragonResourceManager::fortificationSpriteSheet	= new Texture("../assets/Fortification/Fortification_SpriteSheet.png", 2048, 2048, true, true);

TextureSampler * SlayTheDragonResourceManager::archerTorso		= new TextureSampler(new Texture("../assets/hurly-burly/ArcherAssets/ArcherTorso.png", 1024, 1024, true, true), 275, 765);
TextureSampler * SlayTheDragonResourceManager::archerArm		= new TextureSampler(new Texture("../assets/hurly-burly/ArcherAssets/ArcherArm.png", 1024, 1024, true, true), 64, 228);
TextureSampler * SlayTheDragonResourceManager::archerHat		= new TextureSampler(new Texture("../assets/hurly-burly/ArcherAssets/ArcherHat.png", 1024, 1024, true, true), 191, 275);

TextureSampler * SlayTheDragonResourceManager::dragonHead		= new TextureSampler(new Texture("../assets/hurly-burly/DragonAssets/DragonHead.png", 1024, 1024, true, true), 552, 388);
TextureSampler * SlayTheDragonResourceManager::dragonMouth		= new TextureSampler(new Texture("../assets/hurly-burly/DragonAssets/DragonMouth.png", 1024, 1024, true, true), 262, 270);
TextureSampler * SlayTheDragonResourceManager::dragonTorso		= new TextureSampler(new Texture("../assets/hurly-burly/DragonAssets/DragonTorso.png", 1024, 1024, true, true), 417, 797);
TextureSampler * SlayTheDragonResourceManager::dragonUpperWing	= new TextureSampler(new Texture("../assets/hurly-burly/DragonAssets/DragonUpperWing.png", 1024, 1024, true, true), 580, 326);
TextureSampler * SlayTheDragonResourceManager::dragonLowerWing	= new TextureSampler(new Texture("../assets/hurly-burly/DragonAssets/DragonLowerWing.png", 1024, 1024, true, true), 578, 298);

TextureSampler * SlayTheDragonResourceManager::itemBow	= new TextureSampler(new Texture("../assets/hurly-burly/WeaponAssets/Bow.png", 1024, 1024, true, true), 99, 474);
TextureSampler * SlayTheDragonResourceManager::itemArrow	= new TextureSampler(new Texture("../assets/hurly-burly/WeaponAssets/Arrow.png", 1024, 1024, true, true), 77, 539);
TextureSampler * SlayTheDragonResourceManager::itemFireball	= new TextureSampler(new Texture("../assets/hurly-burly/DragonAssets/Fireball.png", 1024, 1024, true, true), 309, 302);
TextureSampler * SlayTheDragonResourceManager::itemNone	= new TextureSampler(new Texture("../assets/hurly-burly/blank.png", 1, 1, true, true), 1, 1);

void SlayTheDragonResourceManager::init(){
	//resources.push_back(splashMessage);
	
	resources.push_back(fortBackground);
	resources.push_back(fortForeground);
	resources.push_back(fortStructure);

	resources.push_back(archerTorso);
	resources.push_back(archerArm);
	resources.push_back(archerHat);
	
	resources.push_back(dragonHead);
	resources.push_back(dragonMouth);
	resources.push_back(dragonTorso);
	resources.push_back(dragonUpperWing);
	resources.push_back(dragonLowerWing);

	resources.push_back(itemBow);
	resources.push_back(itemArrow);
	resources.push_back(itemFireball);
	resources.push_back(itemNone);
}
