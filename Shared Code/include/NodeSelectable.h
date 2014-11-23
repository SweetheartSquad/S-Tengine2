#pragma once

#include <map>

#include "Node.h"

class NodeSelectable abstract : public virtual Node{
public:
	static _Uint32t nextColor;
	static std::map<_Uint32t, NodeSelectable *> pickingMap;

	_Uint32t pickingColor;

	NodeSelectable();
};