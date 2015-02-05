#pragma once

#include "node/NodeUpdatable.h"

class GameJamCharacter;

enum DialogActionType{
	NONE,
	SAY,
	LISTEN
};

class DialogAction : public virtual NodeUpdatable{
public:

	GameJamCharacter * speaker;

	explicit DialogAction(GameJamCharacter * _speaker);
	virtual ~DialogAction();

	virtual DialogActionType getActionType();
	virtual void update(Step* _step) override;
	virtual bool isComplete();
protected:
	bool complete;
};