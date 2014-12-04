#pragma once

#include <vector>

#include <cinder/Vector.h>

#include "Command.h"
#include "Transform.h"

class CMD_MoveSelectedJoints : public Command{
public:
	// Relative move will move by _v, non-relative move will move to _v
	// Local = true: OBJECT 
	// Local = false: WORLD
	CMD_MoveSelectedJoints(ci::Vec3d _v, bool _relative, CoordinateSpace _space);
	~CMD_MoveSelectedJoints(void);

	void execute();
	void unexecute();
private:
	ci::Vec3d v;
	bool relative;
	CoordinateSpace space;
};

