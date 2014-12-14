#pragma once

#include <vector>

#include <cinder/Vector.h>

#include "Command.h"
#include "Transform.h"

class CMD_ScaleSelectedTransformables : public Command{
public:
	// Relative will scale TO _scale, non-relative will scale BY _scale
	CMD_ScaleSelectedTransformables(ci::Vec3f _scale, bool _relative, CoordinateSpace _space);
	~CMD_ScaleSelectedTransformables(void);

	bool execute();
	bool unexecute();
private:
	ci::Vec3f scale;
	bool relative;
	CoordinateSpace space;
};