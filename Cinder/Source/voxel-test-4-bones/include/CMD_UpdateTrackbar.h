#pragma once

#include "Command.h"

class TrackBar;

class CMD_UpdateTrackbar : public Command{
public:
	// Relative move will move _node BY _v, non-relative move will move _node TO _v
	CMD_UpdateTrackbar(TrackBar * _trackbar);
	~CMD_UpdateTrackbar(void);

	bool execute();
	bool unexecute();
private:
	TrackBar * trackbar;
	float oldTargetVal;
	float newTargetVal;
};

