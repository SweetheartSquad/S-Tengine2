#pragma once

#include <HorizontalLinearLayout.h>

HorizontalLinearLayout::HorizontalLinearLayout(BulletWorld* _bulletWorld, Scene* _scene) :
	LinearLayout(_bulletWorld, _scene),
	NodeBulletBody(_bulletWorld)
{
}

float HorizontalLinearLayout::getMeasuredWidth() {
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
	accumulator = accumulator + getMarginLeft() + getMarginRight() + getPaddingLeft() + getPaddingRight();
	return accumulator;
}

float HorizontalLinearLayout::getMeasuredHeight() {
	float max = 0.0f;
	for(unsigned long int i = 0; i < childTransform->children.size(); ++i) {
		Transform * trans = dynamic_cast<Transform *>(childTransform->children.at(i));
		if(trans != nullptr) {
			if(trans->children.size() > 0) {
				NodeUI * node = dynamic_cast<NodeUI *>(trans->children.at(0));
				max = std::max(max, node->getMeasuredHeight());
			}
		}
	}
	max = max + getMarginBottom() + getMarginTop() + getPaddingBottom() + getPaddingTop();
	return max;
}

void HorizontalLinearLayout::update(Step* _step) {
	for (unsigned long int i = 0; i < childTransform->children.size(); ++i){
		if (i == 0) {
			Transform * trans = dynamic_cast<Transform *>(childTransform->children.at(i));
			NodeUI * ui = dynamic_cast<NodeUI * >(trans->children.at(0));
			trans->translate(ui->getMarginLeft(), 0.f, 0.f, false);
		}else {
			Transform * prevTransform = dynamic_cast<Transform *>(childTransform->children.at(i - 1));
			NodeUI * prevUiNode = dynamic_cast<NodeUI *>(prevTransform->children.at(0));

			Transform * trans = dynamic_cast<Transform *>(childTransform->children.at(i));
			NodeUI * ui = dynamic_cast<NodeUI * >(trans->children.at(0));

			float x = prevTransform->getTranslationVector().x 
						+ prevUiNode->getMeasuredWidth()
						+ prevUiNode->getMarginRight() 
						+ ui->getMarginLeft();

			float y = -ui->getMarginTop();
			trans->translate(x, y, 0.f, false);
		}
	}
	NodeUI::update(_step);
}