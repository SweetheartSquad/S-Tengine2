#pragma once

#include <json/json.h>
#include <glm\glm.hpp>

class Configuration{
public:
	bool fullscreen;
	glm::uvec2 resolution;
	double fps;
	signed long int rngSeed;


	Configuration();

	void save(const std::string & _filename);
	void load(const std::string & _filename);
};