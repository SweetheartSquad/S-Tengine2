#include "node/NodeUpdatable.h"

NodeUpdatable::NodeUpdatable() {
	nodeType |= NodeType::kNODE_UPDATABLE;
}