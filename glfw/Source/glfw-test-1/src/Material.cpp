#include "Material.h"
#include <Resource.h>

Material::Material(float shininess, glm::vec3 specularColor, bool _autoRelease) :
	NodeResource(_autoRelease)
{
}

Material::~Material(){
}

void Material::load() {
	//empty
}

void Material::unload() {
	//empty
}