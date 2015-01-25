#pragma once

#include "node/NodeUpdatable.h"

class Character;

enum DialogActionType{
	NONE,
	SAY,
	LISTEN
};

class DialogAction : public virtual NodeUpdatable{
public:

	Character * spreaker;

	explicit DialogAction(Character * _spreaker);
	virtual ~DialogAction();

	virtual DialogActionType getActionType();
	virtual void update(Step* _step) override;
	virtual bool isComplete();
protected:
	bool complete;
};