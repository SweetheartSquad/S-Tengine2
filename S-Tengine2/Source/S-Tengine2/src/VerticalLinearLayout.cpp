#pragma once

#include<VerticalLinearLayout.h>

VerticalLinearLayout::VerticalLinearLayout(BulletWorld* _bulletWorld, Scene* _scene) :
	LinearLayout(_bulletWorld, _scene),
	NodeBulletBody(_bulletWorld)
{
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
						- prevUiNode->getHeight(true, false)
						- ui->getMarginTop()
						- prevUiNode->getMarginBottom();
			trans->translate(x, y, 0.f, false);
		}
	}
}