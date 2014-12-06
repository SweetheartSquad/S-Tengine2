#pragma once

#include "CMD_SelectNodes.h"
#include "UI.h"

CMD_SelectNodes::CMD_SelectNodes(std::vector<Node *> _nodes, bool _additive, bool _subtractive) :
	additive(_additive),
	subtractive(_subtractive),
	nodesForSelection(_nodes)
{
}

CMD_SelectNodes::CMD_SelectNodes(Node * _node, bool _additive, bool _subtractive) :
	additive(_additive),
	subtractive(_subtractive)
{
	if(_node != nullptr){
		nodesForSelection.push_back(_node);
	}
}

CMD_SelectNodes::~CMD_SelectNodes(){
	previousSelectedNodes.clear();
	nodesForSelection.clear();
}

void CMD_SelectNodes::execute(){
	previousSelectedNodes = UI::selectedNodes;
	
	if(nodesForSelection.size() == 0){
		if(!additive && !subtractive){
			UI::selectedNodes.clear();
		}
	}else{
		// Nodes that are in previousSelectedNodes but not nodesForSelection
		std::vector<Node *> oldS;
		// Nodes that are in previousSelectedNodes and nodesForSelection
		std::vector<Node *> overlapS;
		// Nodes that are in nodesForSelection but not previousSelectedNodes
		std::vector<Node *> newS;

		for(unsigned long int i = 0; i < nodesForSelection.size(); ++i){
			bool overlap = false;
			for(unsigned long int j = 0; j < previousSelectedNodes.size(); ++j){
				if(nodesForSelection.at(i) == previousSelectedNodes.at(j)){
					overlap = true;
					break;
				}
			}
			if(overlap){
				overlapS.push_back(nodesForSelection.at(i));
			}else{
				newS.push_back(nodesForSelection.at(i));
			}
		}

		for(unsigned long int i = 0; i < previousSelectedNodes.size(); ++i){
			bool overlap = false;
			for(unsigned long int j = 0; j < overlapS.size(); ++j){
				if(previousSelectedNodes.at(i) == overlapS.at(j)){
					overlap = true;
					break;
				}
			}
			if(!overlap){
				oldS.push_back(previousSelectedNodes.at(i));
			}
		}

		// If subtractive (shift-select, control-select), overlapping selections are ignored
		if(subtractive){
			overlapS.clear();
			if(!additive){
				// If subtractive and not additive (control-select), the new selections are ignored
				newS.clear();
			}else{
				// If subtractive and additive (shift-select), new and old selections are selected
			}
		}else{
			if(!additive){
				// If not subtractive and not additive (normal select), the old selections are ignored
				oldS.clear();
			}else{
				// If not subtractive and additive (shift+control-select), everything is selected
			}
		}

		std::vector<Node *> correctedNodesForSelection;
		correctedNodesForSelection.reserve(oldS.size() + overlapS.size() + newS.size());
		if(oldS.size() != 0){
			correctedNodesForSelection.insert( correctedNodesForSelection.end(), oldS.begin(), oldS.end() );
		}
		if(overlapS.size() != 0){
			correctedNodesForSelection.insert( correctedNodesForSelection.end(), overlapS.begin(), overlapS.end() );
		}
		if(newS.size() != 0){
			correctedNodesForSelection.insert( correctedNodesForSelection.end(), newS.begin(), newS.end() );
		}

		UI::selectedNodes = correctedNodesForSelection;
	}
}

void CMD_SelectNodes::unexecute(){
	UI::selectedNodes = previousSelectedNodes;
}