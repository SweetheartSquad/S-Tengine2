#pragma once

#include <LinearLayout.h>
#include <NodeUI.h>

LinearLayout::LinearLayout(Orientation _orientation):
	Transform(),
	orientation(_orientation)
{
}

LinearLayout::~LinearLayout(){
}

void LinearLayout::update(Step* _step){
	if(updateRequired) {
		if(orientation == HORIZONTAL) {
			for (unsigned long int i = 0; i < children.size(); ++i){
				if (i == 0) {
					//children.at(i).x = children.at(i).marginLeft;
				}else {
					Transform * prevTransform = dynamic_cast<Transform *>(children.at(i - 1));
					NodeUI * prevUiNode = dynamic_cast<NodeUI *>(prevTransform->children.at(0));
					// Add margins
					dynamic_cast<Transform *>(children.at(i))->translate(prevTransform->getTranslationVector().x + prevUiNode->getMeasuredWidth(), 0.f, 0.f, false);
				}
			}
		}else {
			for(unsigned long int i = 0; i < children.size(); ++i){
				if (i == 0) {
					//children.at(i).x = children.at(i).marginLeft;
				}else {
					Transform * prevTransform = dynamic_cast<Transform *>(children.at(i - 1));
					NodeUI * prevUiNode = dynamic_cast<NodeUI *>(prevTransform->children.at(0));
					// Add margins
					dynamic_cast<Transform *>(children.at(i))->translate(0.f, prevTransform->getTranslationVector().y - prevUiNode->getMeasuredHeight(), 0.f, false);
				}
			}
		}
	}
	Transform::update(_step);
}

Transform * LinearLayout::addChild(NodeChild * _child, bool _underNewTransform){
	NodeUI * node = dynamic_cast<NodeUI *>(_child);
	if(node == nullptr){
		throw "Child must be an instance of or extend NodeUI";
	}
	updateRequired = true;
	Transform::addChild(_child, _underNewTransform);
}

void LinearLayout::setOrientation(Orientation _orientation){
	orientation = _orientation;
	updateRequired = true;
}