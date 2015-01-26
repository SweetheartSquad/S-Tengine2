#pragma once

#include <vector>

#include "DialogAction.h"
#include "DialogEvent.h"
#include "SayAction.h"
#include "node/NodeUpdatable.h"
#include "Character.h"

const std::string dialogBits[29] =
{
	"I hear theyre adding a new floor to Dunton Tower. I wonder what its for.",
	"Id watch your back around here, if I were you.",
	"The vending machine never has any apple juice.",
	"I havent slept in two days.",
	"Have you seen the Quad lately? Its beautiful.",
	"All the hallways in Azraeli Pavilion look the same, dont they?",
	"I wonder who built the Tunnels. Maybe they were here already when the school was built",
	"When Im in the computer lab, I can’t tell if its light or dark out.",
	"Do you know if any Food Services are still open?",
	"Havent I seen you around here before?",
	"Standing in the Loeb Building lobby feels like being in an infinite space.",
	"Being here at night always feels really weird.",
	"I hear theres a secret room underneath the Dean’s office.",
	"Do you ever feel like you’ve been here before?",
	"Im not sure if I graduated yet. I just keep going to class anyway.",
	"Have you been to the Lab lately?",
	"Not to be rude, but Im kind of busy here.",
	"Remember to keep hydrated.",
	"I wonder what my future holds.",
	"I think I saw a shadowy figure out in the Quad.",
	"Ive heard people sometimes disappear in the Tunnels.",
	"Im tired of Ottawa weather. Its too hot, then its too cold.",
	"I like being alone in the Archives. Wandering between the sliding shelves is very peaceful.",
	"I met a guy in the hallway around midnight. He gave me a link to his Kickstarter.",
	"I think Iem lost. Please don’t help me.",
	"Where do university programs go when they die?",
	"Youre running out of time.",
	"All ideas are good. All ideas are bad.",
	"Good luck."
};

class DialogHandler : public virtual NodeUpdatable{
public:
	std::vector<Character *> sceneCharacters;
	DialogEvent * dialogEvent;

	DialogHandler(std::vector<Character *> _sceneCharacters);
	~DialogHandler();

	void makeDialog();
	void update(Step* _step) override;
};