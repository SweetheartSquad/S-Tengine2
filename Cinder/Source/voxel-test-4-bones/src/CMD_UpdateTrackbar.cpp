#pragma once

#include "CMD_UpdateTrackbar.h"
#include "TrackBar.h"
#include <sstream>

CMD_UpdateTrackbar::CMD_UpdateTrackbar(TrackBar * _trackbar) :
	trackbar(_trackbar)
{
}

bool CMD_UpdateTrackbar::execute(){
	if(trackbar == nullptr){
		error("Cannot update null trackbar");
		return false;
	}
	if(firstRun){
		oldTargetVal = *trackbar->target;
		trackbar->update(nullptr);
		newTargetVal = *trackbar->target;
	}else{
		*trackbar->target = newTargetVal;
	}
	std::stringstream ss;
	ss << "Time set to " << newTargetVal;
	log(ss.str());
	
	return true;
}

bool CMD_UpdateTrackbar::unexecute(){
	*trackbar->target = oldTargetVal;
	std::stringstream ss;
	ss << "Time reset to " << oldTargetVal;
	log(ss.str());
	return true;
}

CMD_UpdateTrackbar::~CMD_UpdateTrackbar(void){}
