#pragma once

#include <SlayTheDragonResourceManager.h>

TextureSampler * SlayTheDragonResourceManager::splashMessage	= new TextureSampler(new Texture("../assets/hurly-burly/SplashMessages/SlayTheDragon.png", 1024, 1024, true, true), 1024, 1024);

Texture * SlayTheDragonResourceManager::fortificationSpriteSheet = new Texture("../assets/structure components/fortification/fortification_SpriteSheet.png", 2048, 2048, true, true);

TextureSampler * SlayTheDragonResourceManager::archerTorso		= new TextureSampler(new Texture("../assets/hurly-burly/ArcherAssets/archerBreastplate.png", 1024, 1024, true, true), 268, 776);
TextureSampler * SlayTheDragonResourceManager::archerArm		= new TextureSampler(new Texture("../assets/hurly-burly/ArcherAssets/archerArm.png", 1024, 1024, true, true), 92, 327);
TextureSampler * SlayTheDragonResourceManager::archerHelmet		= new TextureSampler(new Texture("../assets/hurly-burly/ArcherAssets/archerHelmet.png", 1024, 1024, true, true), 233, 254);

TextureSampler * SlayTheDragonResourceManager::itemBow	= new TextureSampler(new Texture("../assets/hurly-burly/WeaponAssets/Bow.png", 1024, 1024, true, true), 230, 395);
TextureSampler * SlayTheDragonResourceManager::itemArrow	= new TextureSampler(new Texture("../assets/hurly-burly/WeaponAssets/Arrow.png", 1024, 1024, true, true), 83, 515);
TextureSampler * SlayTheDragonResourceManager::itemFireball	= new TextureSampler(new Texture("../assets/hurly-burly/WeaponAssets/Fireball.png", 1024, 1024, true, true), 83, 515);

void SlayTheDragonResourceManager::init(){

}
