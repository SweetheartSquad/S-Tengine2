#pragma once
class Command
{
public:
	virtual void execute() = 0;
	virtual void unexecute() = 0;

};

