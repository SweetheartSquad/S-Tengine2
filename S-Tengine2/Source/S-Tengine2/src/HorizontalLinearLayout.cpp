#pragma once

#include <HorizontalLinearLayout.h>

HorizontalLinearLayout::HorizontalLinearLayout(BulletWorld* _bulletWorld, Scene* _scene) :
	LinearLayout(_bulletWorld, _scene),
	NodeBulletBody(_bulletWorld)
{
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
						+ prevUiNode->getWidth(true, false)
						+ prevUiNode->getMarginRight() 
						+ ui->getMarginLeft();

			float y = -ui->getMarginTop();
			trans->translate(x, y, 0.f, false);
		}
	}
	NodeUI::update(_step);
}