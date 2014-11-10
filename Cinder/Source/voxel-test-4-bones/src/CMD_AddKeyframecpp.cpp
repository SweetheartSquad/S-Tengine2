#pragma once

#include "CMD_AddKeyframe.h"

#include <algorithm>


#include <cinder/app/AppBasic.h>

CMD_AddKeyframe::CMD_AddKeyframe(std::vector<Keyframe> * _keyframes, float _time, float _value, Easing::Type _interpolation) :
	keyframes(_keyframes),
	keyframe(Keyframe(0,0,_interpolation)),
	followingStartValue(NULL)
{
	ci::app::console() << "TIME: " << _time << std::endl;
	// Find insert iterator and set startValue of new keyframe
	std::vector<Keyframe>::iterator followingKeyframe_it = std::upper_bound(keyframes->begin(),keyframes->end(),keyframe,Keyframe::keyframe_compare);

	// If a previous keyframe exists
	if(followingKeyframe_it != keyframes->end() && followingKeyframe_it->time != keyframes->front().time){
		// get previous keyframe
		std::vector<Keyframe>::iterator previousKeyframe;
		previousKeyframe = std::prev(followingKeyframe_it);
		ci::app::console() << "previous frame: " << previousKeyframe->time << std::endl;
		// calculate and set delta values
		keyframe.deltaTime = keyframe.deltaTime - previousKeyframe->deltaTime;
		keyframe.deltaValue = keyframe.deltaValue - previousKeyframe->deltaValue;
	}else{
		// set delta values
		ci::app::console() << "no previous frame" << std::endl;
		keyframe.deltaTime = 0;
		keyframe.deltaValue = 0;
	}

	// Get deltaTime and deltaValue of next keyframe
	if(followingKeyframe_it != keyframes->end()){
		nextDeltaTime = followingKeyframe_it->deltaTime;
		nextDeltaValue = followingKeyframe_it->deltaValue;
	}
}

void CMD_AddKeyframe::execute(){
	ci::app::console() << "execute CMD_AddKeyframe" << std::endl;
	
	// Add keyframe
	std::vector<Keyframe>::iterator followingKeyframe_it = std::upper_bound(keyframes->begin(),keyframes->end(),keyframe,Keyframe::keyframe_compare);
	keyframes->insert(followingKeyframe_it,keyframe);

	// Get the new iterator after insert
	followingKeyframe_it = std::upper_bound(keyframes->begin(),keyframes->end(),keyframe,Keyframe::keyframe_compare);

	// Change next keyframe's start value
	if (followingKeyframe_it != keyframes->end()){
		// d2 - d1
		followingKeyframe_it->startValue = keyframe.value;
	}
}

void CMD_AddKeyframe::unexecute(){
	// Remove keyframe
	for(unsigned long int i = 0; i < keyframes->size(); ++i){
		if(keyframes->at(i).time == keyframe.time){
			keyframes->erase(keyframes->begin() + i);
			break;
		}
	}

	std::vector<Keyframe>::iterator followingKeyframe_it = std::upper_bound(keyframes->begin(),keyframes->end(),keyframe,Keyframe::keyframe_compare);

	// Restore next keyframe's start value
	if (followingKeyframe_it != keyframes->end()){
		followingKeyframe_it->startValue = followingStartValue;
	}
}

CMD_AddKeyframe::~CMD_AddKeyframe()
{
}