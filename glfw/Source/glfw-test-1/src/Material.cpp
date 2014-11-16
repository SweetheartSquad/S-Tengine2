#include "Material.h"
#include <Resource.h>

Material::Material(TypeName _materialType, bool _autoRelease) :
	NodeResource(_autoRelease)
{
	data.type = _materialType;
}

Material::~Material(){
}

void Material::load() {
	//empty
}

void Material::unload() {
	//empty
}