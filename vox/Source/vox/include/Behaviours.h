#pragma once

class PuppetCharacter;

class Behaviours{
public:
	static void followX(PuppetCharacter * b);
	static void attackX(PuppetCharacter * b);
	static void patrolX(PuppetCharacter * b);
};