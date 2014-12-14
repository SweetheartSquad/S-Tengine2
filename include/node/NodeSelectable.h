#pragma once

#include <map>
#include <vector>

#include "Node.h"

class NodeSelectable abstract : public virtual Node{
public:
	static _Uint32t nextColor;
	static std::map<_Uint32t, NodeSelectable *> pickingMap;
	static std::vector<_Uint32t> reserved;

	_Uint32t pickingColor;

	NodeSelectable();
};