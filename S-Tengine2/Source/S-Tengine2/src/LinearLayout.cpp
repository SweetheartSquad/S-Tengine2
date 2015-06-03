#pragma once

#include <LinearLayout.h>
#include <NodeUI.h>


LinearLayout::LinearLayout(Orientation _orientation, BulletWorld* _world, Scene* _scene):
	NodeUI(_world, _scene),
	NodeBulletBody(_world),
	orientation(_orientation)
{
	updateColider();
}

LinearLayout::~LinearLayout(){
}

void LinearLayout::update(Step* _step){
	if(layoutDirty) {
		if(orientation == HORIZONTAL) {
			for (unsigned long int i = 0; i < childTransform->children.size(); ++i){
				if (i == 0) {
					Transform * trans = dynamic_cast<Transform *>(childTransform->children.at(i));
					NodeUI * ui = dynamic_cast<NodeUI * >(trans->children.at(0));
					trans->translate(ui->getMarginLeft(), 0.f, 0.f);
				}else {
					Transform * prevTransform = dynamic_cast<Transform *>(childTransform->children.at(i - 1));
					NodeUI * prevUiNode = dynamic_cast<NodeUI *>(prevTransform->children.at(0));
					// Add margins

					Transform * trans = dynamic_cast<Transform *>(childTransform->children.at(i));
					NodeUI * ui = dynamic_cast<NodeUI * >(trans->children.at(0));
					trans->translate(ui->getMarginLeft(), 0.f, 0.f);

					float x = prevTransform->getTranslationVector().x + prevUiNode->getMeasuredWidth();
					float y = ui->getMarginTop() + prevUiNode->getMarginBottom();
					dynamic_cast<Transform *>(trans)->translate(x, y, 0.f, false);
				}
			}
		}else {
			for(unsigned long int i = 0; i < childTransform->children.size(); ++i){
				if (i == 0) {
					Transform * trans = dynamic_cast<Transform *>(childTransform->children.at(i));
					NodeUI * ui = dynamic_cast<NodeUI * >(trans->children.at(0));
					trans->translate(0.f, -ui->getMarginTop(), 0.f);
				}else {
					Transform * prevTransform = dynamic_cast<Transform *>(childTransform->children.at(i - 1));
					NodeUI * prevUiNode = dynamic_cast<NodeUI *>(prevTransform->children.at(0));
					// Add margins

					Transform * trans = dynamic_cast<Transform *>(childTransform->children.at(i));
					NodeUI * ui = dynamic_cast<NodeUI * >(trans->children.at(0));
					trans->translate(ui->getMarginLeft(), 0.f, 0.f);

					float x = ui->getMarginLeft();
					float y = prevTransform->getTranslationVector().y - prevUiNode->getMeasuredHeight() - ui->getMarginTop() - prevUiNode->getMarginBottom();
					dynamic_cast<Transform *>(trans)->translate(x, y, 0.f, false);
				}
			}
		}
		//updateColider();
		//updateRequired = false;
	}
	NodeUI::update(_step);
}

float LinearLayout::getMeasuredWidth(){
	float accumulator = 0.0f;
	for(unsigned long int i = 0; i < childTransform->children.size(); ++i) {
		Transform * trans = dynamic_cast<Transform *>(childTransform->children.at(i));
		if(trans != nullptr) {
			if(trans->children.size() > 0) {
				NodeUI * node = dynamic_cast<NodeUI *>(trans->children.at(0));
				accumulator += node->getMeasuredWidth();
			}
		}
	}
	return accumulator;
}

float LinearLayout::getMeasuredHeight(){
	float accumulator = 0.0f;
	for(unsigned long int i = 0; i < childTransform->children.size(); ++i) {
		Transform * trans = dynamic_cast<Transform *>(childTransform->children.at(i));
		if(trans != nullptr) {
			if(trans->children.size() > 0) {
				NodeUI * node = dynamic_cast<NodeUI *>(trans->children.at(0));
				accumulator += node->getMeasuredHeight();
			}
		}
	}
	return accumulator;
}

Transform * LinearLayout::addChild(NodeUI* _uiElement){
	layoutDirty = true;
	return childTransform->addChild(_uiElement);
}

void LinearLayout::setOrientation(Orientation _orientation){
	orientation = _orientation;
	layoutDirty = true;
}