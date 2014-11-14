#pragma once

#include "NodeSelectable.h"

_Uint32t NodeSelectable::nextColor = 0xFFFFFF;
std::map<_Uint32t, NodeSelectable *> NodeSelectable::pickingMap;

NodeSelectable::NodeSelectable() :
	pickingColor(nextColor)
{
	pickingMap.insert(std::pair<_Uint32t, NodeSelectable *>(nextColor, this));
	nextColor -= 0x000001;
}