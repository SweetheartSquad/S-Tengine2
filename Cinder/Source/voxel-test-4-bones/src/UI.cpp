#pragma once

#include "UI.h"
#include "Easing.h"
#include "Joint.h"

std::vector<Node *> UI::selectedNodes;

float UI::time;
float UI::stepScale = 0.1f;
int UI::interpolationValue = 0;
std::vector<std::string> UI::interpolationNames = getInterpolationNames();

ci::Vec3d UI::handlePos = ci::Vec3d(0, 0, 0);
ci::Vec3d UI::displayHandlePos = ci::Vec3d(0, 0, 0);

void UI::updateHandlePos(bool _displayOnly){
	displayHandlePos.set(0,0,0);
	for(unsigned long int i = 0; i < UI::selectedNodes.size(); ++i){
		Joint * j = dynamic_cast<Joint *>(UI::selectedNodes.at(i));
		if(j != NULL){
			glm::vec3 t = j->getPos(false);
			displayHandlePos += Vec3f(t.x, t.y, t.z);
		}
	}
	displayHandlePos /= UI::selectedNodes.size();

	if(!_displayOnly){
		handlePos = displayHandlePos;
	}
}

// I don't know how else or where to put this
std::vector<std::string> UI::getInterpolationNames(){
	std::vector<std::string> _interpolations;

	_interpolations.push_back("kNONE");
	_interpolations.push_back("kEASE_IN_QUAD");
	_interpolations.push_back("kEASE_OUT_QUAD");
	_interpolations.push_back("kEASE_IN_OUT_QUAD");
	_interpolations.push_back("kEASE_IN_CUBIC");
	_interpolations.push_back("kEASE_OUT_CUBIC");
	_interpolations.push_back("kEASE_IN_OUT_CUBIC");
	_interpolations.push_back("kEASE_IN_QUART");
	_interpolations.push_back("kEASE_OUT_QUART");
	_interpolations.push_back("kEASE_IN_OUT_QUART");
	_interpolations.push_back("kEASE_IN_QUINT");
	_interpolations.push_back("kEASE_OUT_QUINT");
	_interpolations.push_back("kEASE_IN_OUT_QUINT");
	_interpolations.push_back("kEASE_IN_SINE");
	_interpolations.push_back("kEASE_OUT_SINE");
	_interpolations.push_back("kEASE_IN_OUT_SINE");
	_interpolations.push_back("kEASE_IN_EXPO");
	_interpolations.push_back("kEASE_OUT_EXPO");
	_interpolations.push_back("kEASE_IN_OUT_EXPO");
	_interpolations.push_back("kEASE_IN_CIRC");
	_interpolations.push_back("kEASE_OUT_CIRC");
	_interpolations.push_back("kEASE_IN_OUT_CIRC");
	_interpolations.push_back("kEASE_IN_ELASTIC");
	_interpolations.push_back("kEASE_OUT_ELASTIC");
	_interpolations.push_back("kEASE_IN_OUT_ELASTIC");
	_interpolations.push_back("kEASE_IN_BOUNCE");
	_interpolations.push_back("kEASE_OUT_BOUNCE");
	_interpolations.push_back("kEASE_IN_OUT_BOUNCE");
	_interpolations.push_back("kEASE_IN_BACK");
	_interpolations.push_back("kEASE_OUT_BACK");
	_interpolations.push_back("kEASE_IN_OUT_BACK");

	return _interpolations;
}
