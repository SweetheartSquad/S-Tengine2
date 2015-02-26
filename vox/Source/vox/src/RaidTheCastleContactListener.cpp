#include "RaidTheCastleContactListener.h"
#include "PuppetScene.h"

#include "Catapult.h"
#include "PuppetCharacter.h"
#include "Box2DSprite.h"
#include <Box2D/Box2D.h>
#include <Box2D\Dynamics\Joints\b2RevoluteJoint.h>

using namespace std;

/*bool GameJamContactListener:: ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB){
	if(fixtureA->GetFilterData().maskBits == 1 || fixtureB->GetFilterData().maskBits == 1){
		// collide
	}else{
		// pass
	}
}*/

void RaidTheCastleContactListener::BeginContact(b2Contact* contact){

	if(contact->GetFixtureA()->IsSensor() || contact->GetFixtureB()->IsSensor()){
		b2Filter fA = contact->GetFixtureA()->GetFilterData();
		b2Filter fB = contact->GetFixtureB()->GetFilterData();

		if(fA.categoryBits == PuppetScene::PLAYER || fB.categoryBits == PuppetScene::PLAYER){

			if(fA.categoryBits == PuppetScene::PLAYER && fB.categoryBits == PuppetScene::PLAYER){
				// Player-Player collision
				playerPlayerContact(contact);
			}else if(fA.categoryBits == PuppetScene::ITEM || fB.categoryBits == PuppetScene::ITEM){
				// Player-Item collision
				playerItemContact(contact);
			}else if(fA.categoryBits == PuppetScene::STRUCTURE || fB.categoryBits == PuppetScene::STRUCTURE){
				// Player-Structure collision
				playerStructureContact(contact);
			}else if(fA.categoryBits == PuppetScene::GROUND || fB.categoryBits == PuppetScene::GROUND){
				// Player-Structure collision
				playerGroundContact(contact);
			}
		}
	}else{
		// do nothing
	}
}

void RaidTheCastleContactListener::playerPlayerContact(b2Contact * contact){
	b2Fixture * fxA = contact->GetFixtureA();
	b2Fixture * fxB = contact->GetFixtureB();

	PuppetCharacter * puppetA = reinterpret_cast<PuppetCharacter *>( fxA->GetBody()->GetUserData() );
    PuppetCharacter * puppetB = reinterpret_cast<PuppetCharacter *>( fxB->GetBody()->GetUserData() );

	if(puppetA != nullptr && puppetB != nullptr){
		// Stuff
	}
}

void RaidTheCastleContactListener::playerItemContact(b2Contact * contact){
	b2Fixture * fxA = contact->GetFixtureA();
	b2Fixture * fxB = contact->GetFixtureB();

	/*
	PuppetCharacter * puppet;
	Item * item;

	puppet = static_cast<PuppetCharacter *>( fxA->GetBody()->GetUserData() );
	if(puppet != nullptr){
		item = static_cast<Item *>( fxB->GetBody()->GetUserData() );
	}else{
		puppet = static_cast<PuppetCharacter *>( fxB->GetBody()->GetUserData() );
		item = static_cast<Item *>( fxA->GetBody()->GetUserData() );
	}

	if(puppet != nullptr && item != nullptr){
		// Stuff
	}*/
}

void RaidTheCastleContactListener::playerStructureContact(b2Contact * contact){
	b2Fixture * fxA = contact->GetFixtureA();
	b2Fixture * fxB = contact->GetFixtureB();
	
	PuppetCharacter * puppet;
	Catapult * catapult;
	float flingerMass;

	puppet = static_cast<PuppetCharacter *>( fxA->GetUserData() );
	if(puppet != nullptr){
		catapult = static_cast<Catapult *>( fxB->GetUserData() );
	}else{
		puppet = static_cast<PuppetCharacter *>( fxB->GetUserData() );
		catapult = static_cast<Catapult *>( fxA->GetUserData() );
	}
	
	if(puppet != nullptr && catapult != nullptr){
		// Check for cooldown
		if(catapult->components.size() > 1){
			// Fire catapult
			b2RevoluteJoint * j = (b2RevoluteJoint *)(*catapult->components.at(1))->body->GetJointList()->joint;
			j->SetMotorSpeed(-j->GetJointAngle()*360);
			j->SetMaxMotorTorque((*catapult->components.at(0))->body->GetMass()*750*(std::abs(j->GetJointAngle())*5));
		}
	}
	//cout << "Player-Player Contact" << endl;
}

void RaidTheCastleContactListener::playerGroundContact(b2Contact* _contact){
	b2Fixture * fxA = _contact->GetFixtureA();
	b2Fixture * fxB = _contact->GetFixtureB();
	
	PuppetCharacter * puppet;

	if(fxA->GetFilterData().categoryBits == PuppetScene::PLAYER){
		puppet = static_cast<PuppetCharacter *>( fxA->GetUserData());
		if(puppet != nullptr){
			puppet->canJump = true;
		}
	}else if(fxB->GetFilterData().categoryBits == PuppetScene::PLAYER){
		puppet = static_cast<PuppetCharacter *>( fxB->GetUserData());
		if(puppet != nullptr){
			puppet->canJump = true;
		}
	}
}

void RaidTheCastleContactListener::EndContact(b2Contact* contact){
	if(contact->GetFixtureA()->IsSensor() || contact->GetFixtureB()->IsSensor()){
		b2Filter fA = contact->GetFixtureA()->GetFilterData();
		b2Filter fB = contact->GetFixtureB()->GetFilterData();

		if((fA.maskBits == 2 + 4 && fB.maskBits == 4) || (fA.maskBits = 4 && fB.maskBits == 2 + 4)){
			// Player character and character
			int i = 0;
		}else if((fA.maskBits == 2 + 4 && fB.maskBits == 4) || (fA.maskBits == 4 && fB.maskBits == 2 + 4)){
			// player character and prop
			int i = 1;
		}else{

		}
	}else{
		// do nothing
	}
}