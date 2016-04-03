#pragma once

#include <json/json.h>
#include <glm\glm.hpp>

class Configuration{
public:
	bool fullscreen;
	glm::uvec2 resolution;
	double fps;
	signed long int rngSeed;
	signed long int monitor;
	std::string title;
	bool useLibOVR;
	bool nodeCounting;

	std::map<std::string, float> gain;

	Configuration();

	void save(const std::string & _filename);
	void load(const std::string & _filename);
};