#pragma once

#include "UI.h"
#include "Easing.h"
#include "Joint.h"

std::vector<Node *> UI::selectedNodes;

float UI::time;
float UI::stepScale = 0.1;
Easing::Type UI::interpolation;

ci::Vec3d UI::handlePos = ci::Vec3d(0, 0, 0);
ci::Vec3d UI::displayHandlePos = ci::Vec3d(0, 0, 0);

void UI::updateHandlePos(bool _displayOnly){
	displayHandlePos.set(0,0,0);
	for(unsigned long int i = 0; i < UI::selectedNodes.size(); ++i){
		Joint * j = dynamic_cast<Joint *>(UI::selectedNodes.at(i));
		if(j != NULL){
			displayHandlePos += j->getPos(false);
		}
	}
	displayHandlePos /= UI::selectedNodes.size();

	if(!_displayOnly){
		handlePos = displayHandlePos;
	}
}
