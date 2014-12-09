#pragma once

#include <vector>

#include <cinder/Vector.h>

#include "Command.h"
#include "Transform.h"

class CMD_ScaleSelectedTransformables : public Command{
public:
	// Relative move will move by _v, non-relative move will move to _v
	CMD_ScaleSelectedTransformables(ci::Vec3f _v, CoordinateSpace _space);
	~CMD_ScaleSelectedTransformables(void);

	void execute();
	void unexecute();
private:
	ci::Vec3f v;
	CoordinateSpace space;
};