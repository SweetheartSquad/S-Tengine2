#pragma once

#include <vector>

#include <cinder/Vector.h>
#include <glm\glm.hpp>

#include "Command.h"
#include "Transform.h"

class CMD_TranslateSelectedTransformables : public Command{
public:
	// Relative will move BY _v, non-relative will move TO _v
	CMD_TranslateSelectedTransformables(ci::Vec3d _v, bool _relative, CoordinateSpace _space);
	~CMD_TranslateSelectedTransformables(void);

	bool execute();
	bool unexecute();
private:
	ci::Vec3d v;
	bool relative;
	CoordinateSpace space;
};

