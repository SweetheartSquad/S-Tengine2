#pragma once

#include <vector>
#include "Command.h"

class Node;
class NodeHierarchical;
class NodeChild;
class Joint;

class CMD_DeleteJoint : public Command{
public:
	CMD_DeleteJoint(std::vector<Joint *> * joints);
	~CMD_DeleteJoint(void);

	void execute();
	void unexecute();

private:
	std::vector<Joint *> * joints;
	
	// The joints which this command is in reference to
	Joint * jointForDeletion;
	
	// Location in jointForDeletion's parent's list of children (or the joint list if it was a root) at which jointForDeletion exists
	std::vector<unsigned long int> indices;

	// If jointForDeletion had children, a copy of them are stored here
	std::vector<std::vector<NodeChild *>> children;
};