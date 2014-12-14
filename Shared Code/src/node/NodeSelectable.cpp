#pragma once

#include "node/NodeSelectable.h"

_Uint32t NodeSelectable::nextColor = 0xFFFFFF;
std::map<_Uint32t, NodeSelectable *> NodeSelectable::pickingMap;

std::vector<_Uint32t> NodeSelectable::reserved = [] {
    std::vector<_Uint32t> v; 
    v.push_back(0xFFFFFF);
    v.push_back(0xFFFF00);
    v.push_back(0xFF0000);
    v.push_back(0x00FF00); 
    v.push_back(0x0000FF); 
    v.push_back(0x000000);
    return v;
}();

NodeSelectable::NodeSelectable()
{
	bool valid;
	do{
		valid = true;
		for(unsigned long int i = 0; i < reserved.size(); ++i){
			if(nextColor == reserved.at(i)){
				valid = false;
				nextColor -= 0x000001;
				break;
			}
		}
	}while(!valid);

	pickingColor = nextColor;
	pickingMap.insert(std::pair<_Uint32t, NodeSelectable *>(pickingColor, this));
	nextColor -= 0x000001;
}