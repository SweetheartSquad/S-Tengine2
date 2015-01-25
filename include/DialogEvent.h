#pragma once

#include <vector>

#include "DialogAction.h"
#include "node/NodeUpdatable.h"

class DialogEvent : public virtual NodeUpdatable{
public:
	std::vector<DialogAction *> actions;
	int currentAction;
	bool running;

	DialogEvent();
	~DialogEvent();

	void update(Step* _step) override;
	void addAction(DialogAction * _action);
};