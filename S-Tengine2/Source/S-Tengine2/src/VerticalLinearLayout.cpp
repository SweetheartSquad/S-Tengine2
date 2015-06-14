#pragma once

#include<VerticalLinearLayout.h>

VerticalLinearLayout::VerticalLinearLayout(BulletWorld* _bulletWorld, Scene* _scene) :
	LinearLayout(_bulletWorld, _scene),
	NodeBulletBody(_bulletWorld)
{
}

float VerticalLinearLayout::getMeasuredWidth() {
	float max = 0.0f;
	for(unsigned long int i = 0; i < childTransform->children.size(); ++i) {
		Transform * trans = dynamic_cast<Transform *>(childTransform->children.at(i));
		if(trans != nullptr) {
			if(trans->children.size() > 0) {
				NodeUI * node = dynamic_cast<NodeUI *>(trans->children.at(0));
				max = std::max(max, node->getMeasuredWidth());
			}
		}
	}
	max = max + getMarginLeft() + getMarginRight() + getPaddingLeft() + getPaddingRight();
	return max;
}

float VerticalLinearLayout::getMeasuredHeight() {
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
	accumulator = accumulator + getMarginBottom() + getMarginTop() + getPaddingBottom() + getPaddingTop();
	return accumulator; 
}

void VerticalLinearLayout::update(Step* _step) {
	for(unsigned long int i = 0; i < childTransform->children.size(); ++i){
		if (i == 0) {
			Transform * trans = dynamic_cast<Transform *>(childTransform->children.at(i));
			NodeUI * ui = dynamic_cast<NodeUI * >(trans->children.at(0));
			trans->translate(0.f, -ui->getMarginTop(), 0.f, false);
		}else {
			Transform * prevTransform = dynamic_cast<Transform *>(childTransform->children.at(i - 1));
			NodeUI * prevUiNode = dynamic_cast<NodeUI *>(prevTransform->children.at(0));

			Transform * trans = dynamic_cast<Transform *>(childTransform->children.at(i));
			NodeUI * ui = dynamic_cast<NodeUI * >(trans->children.at(0));

			float x = ui->getMarginLeft();
			float y = prevTransform->getTranslationVector().y 
						- prevUiNode->getMeasuredHeight()
						- ui->getMarginTop()
						- prevUiNode->getMarginBottom();
			trans->translate(x, y, 0.f, false);
		}
	}
}