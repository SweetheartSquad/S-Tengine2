#pragma once
#include "UI.h"
#include "Easing.h"
#include "Transform.h"
#include "NodeTransformable.h"

std::vector<Node *> UI::selectedNodes;

float UI::time;
Easing::Type UI::interpolation;