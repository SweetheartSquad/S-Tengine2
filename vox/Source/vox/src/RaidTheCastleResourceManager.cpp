#pragma once

#include "RaidTheCastleResourceManager.h"

Texture * castle;

TextureSampler * RaidTheCastleResourceManager::catapultArm  = new TextureSampler(new Texture("../assets/structure components/catapult/CatapultFlinger.png", 512, 512, true, true), 429, 76);;
TextureSampler * RaidTheCastleResourceManager::catapultBody = new TextureSampler(new Texture("../assets/structure components/catapult/CatapultBase.png", 512, 512, true, true), 418, 264);
TextureSampler * RaidTheCastleResourceManager::boulder		= new TextureSampler(new Texture("../assets/structure components/catapult/Boulder1.png", 512, 512, true, true), 108, 103);

RaidTheCastleResourceManager::RaidTheCastleResourceManager(){
}