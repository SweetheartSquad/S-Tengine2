#pragma once

#include "RaidTheCastleResourceManager.h"

Texture * RaidTheCastleResourceManager::castleSpriteSheet    = new Texture("../assets/structure components/castle/Castle_SpriteSheet.png", 2048, 2048, true, true);

TextureSampler * RaidTheCastleResourceManager::catapultArm   = new TextureSampler(new Texture("../assets/structure components/catapult/CatapultFlinger.png", 512, 512, true, true), 429, 76);;
TextureSampler * RaidTheCastleResourceManager::catapultBody  = new TextureSampler(new Texture("../assets/structure components/catapult/CatapultBase.png", 512, 512, true, true), 418, 264);
TextureSampler * RaidTheCastleResourceManager::boulder		 = new TextureSampler(new Texture("../assets/structure components/catapult/Boulder1.png", 512, 512, true, true), 108, 103);
TextureSampler * RaidTheCastleResourceManager::castleBase    = new TextureSampler(new Texture("../assets/structure components/castle/CastleNorm_State1.png", 1024, 1024, true, true), 973, 619);

RaidTheCastleResourceManager::RaidTheCastleResourceManager(){
	resources.push_back(catapultArm);
	resources.push_back(catapultBody);
	resources.push_back(boulder);
}