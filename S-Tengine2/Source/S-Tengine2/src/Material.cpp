#include "Material.h"
#include <Resource.h>

MaterialData::MaterialData(float _shininess, glm::vec3 _specularColor) :
	shininess(_shininess),
	specularColor(_specularColor)
{
}

Material::Material(float _shininess, glm::vec3 _specularColor, bool _autoRelease) :
	NodeResource(_autoRelease),
	data(_shininess, _specularColor)
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