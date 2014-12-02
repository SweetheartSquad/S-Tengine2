#pragma once

#include <vector>

#include <cinder/Vector.h>

#include "Command.h"

class CMD_ScaleSelectedTransformable : public Command{
public:
	// Relative move will move by _v, non-relative move will move to _v
	CMD_ScaleSelectedTransformable(ci::Vec3f _v);
	~CMD_ScaleSelectedTransformable(void);

	void execute();
	void unexecute();
private:
	ci::Vec3f v;
};