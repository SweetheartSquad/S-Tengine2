#pragma once

#include <vector>

#include "Command.h"

class Node;
class NodeHierarchical;
class NodeChild;

class CMD_DeleteJoints : public Command{
public:
	CMD_DeleteJoints(std::vector<NodeHierarchical *> * joints);
	~CMD_DeleteJoints(void);

	void execute();
	void unexecute();

private:
	std::vector<NodeHierarchical *> * joints;
	
	// The joints which this command is in reference to
	std::vector<Node *> jointsForDeletion;
	// The joints which have been deleted by this command
	std::vector<NodeHierarchical *> deletedJoints;
	
	// Location in jointForDeletion's parent's list of children (or the joint list if it was a root) at which jointForDeletion exists
	std::vector<unsigned long int> index;

	// If jointForDeletion had children, a copy of them are stored here
	std::vector<std::vector<NodeChild *>> children;
};