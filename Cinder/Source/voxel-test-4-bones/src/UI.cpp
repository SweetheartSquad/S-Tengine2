#pragma once
#include "UI.h"
#include "Easing.h"
#include "Transform.h"
#include "NodeTransformable.h"

std::vector<Node *> UI::selectedNodes;

float UI::time;
Easing::Type UI::interpolation;

UI::frame_type UI::properties;

void UI::setPropertyWindow(){
	if(UI::selectedNodes.size() != 0){

		if(1 == 1){ // if is nodetransformable
			properties.transform = dynamic_cast<NodeTransformable *>(UI::selectedNodes.at(UI::selectedNodes.size() - 1))->transform;
		}

	}else{
		properties.transform = nullptr;
	}
}