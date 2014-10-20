#pragma once

#include "CMD_SetParent.h"
#include "UI.h"
#include "Node.h"
#include "Joint.h"

CMD_SetParent::CMD_SetParent(/*std::vector<Joint *> * _joints*/)/* :
	joints(_joints)*/
{
}

void CMD_SetParent::execute(){
	//Joint * j = (Joint *)UI::selectedNode;
}

void CMD_SetParent::unexecute(){
	
}

CMD_SetParent::~CMD_SetParent(void){
}