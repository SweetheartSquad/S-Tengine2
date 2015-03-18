#pragma once

#include <SlayTheDragonResourceManager.h>

TextureSampler * SlayTheDragonResourceManager::splashMessage	= new TextureSampler(new Texture("../assets/hurly-burly/SplashMessages/SlayTheDragon.png", 1024, 1024, true, true), 1024, 1024);

Texture * SlayTheDragonResourceManager::fortificationSpriteSheet = new Texture("../assets/structure components/fortification/fortification_SpriteSheet.png", 2048, 2048, true, true);

TextureSampler * SlayTheDragonResourceManager::archerTorso		= new TextureSampler(new Texture("../assets/hurly-burly/ArcherAssets/archerBreastplate.png", 1024, 1024, true, true), 268, 776);
TextureSampler * SlayTheDragonResourceManager::archerArm		= new TextureSampler(new Texture("../assets/hurly-burly/ArcherAssets/archerArm.png", 1024, 1024, true, true), 92, 327);
TextureSampler * SlayTheDragonResourceManager::archerHelmet		= new TextureSampler(new Texture("../assets/hurly-burly/ArcherAssets/archerHelmet.png", 1024, 1024, true, true), 233, 254);

TextureSampler * SlayTheDragonResourceManager::dragonHead		= new TextureSampler(new Texture("../assets/hurly-burly/DragonAssets/dragonHead.png", 1024, 1024, true, true), 268, 776);
TextureSampler * SlayTheDragonResourceManager::dragonMouth		= new TextureSampler(new Texture("../assets/hurly-burly/DragonAssets/dragonMouth.png", 1024, 1024, true, true), 268, 776);
TextureSampler * SlayTheDragonResourceManager::dragonTorso		= new TextureSampler(new Texture("../assets/hurly-burly/DragonAssets/dragonTorso.png", 1024, 1024, true, true), 92, 327);
TextureSampler * SlayTheDragonResourceManager::dragonUpperWing	= new TextureSampler(new Texture("../assets/hurly-burly/DragonAssets/dragonUpperWing.png", 1024, 1024, true, true), 233, 254);
TextureSampler * SlayTheDragonResourceManager::dragonLowerWing	= new TextureSampler(new Texture("../assets/hurly-burly/DragonAssets/dragonLowerWing.png", 1024, 1024, true, true), 268, 776);

TextureSampler * SlayTheDragonResourceManager::itemBow	= new TextureSampler(new Texture("../assets/hurly-burly/WeaponAssets/Bow.png", 1024, 1024, true, true), 230, 395);
TextureSampler * SlayTheDragonResourceManager::itemArrow	= new TextureSampler(new Texture("../assets/hurly-burly/WeaponAssets/Arrow.png", 1024, 1024, true, true), 83, 515);
TextureSampler * SlayTheDragonResourceManager::itemFireball	= new TextureSampler(new Texture("../assets/hurly-burly/DragonAssets/Fireball.png", 1024, 1024, true, true), 83, 515);
TextureSampler * SlayTheDragonResourceManager::itemNone	= new TextureSampler(new Texture("../assets/hurly-burly/blank.png", 1, 1, true, true), 1, 1);

void SlayTheDragonResourceManager::init(){

}
