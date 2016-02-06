#pragma once

#include <regex>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>

#include <json/json.h>

#include "MeshInterface.h"
#include "Resource.h"
#include "FileUtils.h"
#include "Animation.h"
#include "Box2DSprite.h"
#include "Box2DWorld.h"
#include "Texture.h"

#include <tiny_obj_loader.h>

std::vector<TriMesh *> Resource::loadMeshFromObj(std::string _objSrc, bool _autorelease){

	std::string inputfile = sweet::FileUtils::readFile(_objSrc);
	
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err = tinyobj::LoadObj(shapes, materials, _objSrc.c_str());
	
	if (!err.empty()) {
	  std::cerr << err << std::endl;
	  exit(1);
	}
	//std::cout << "# of shapes    : " << shapes.size() << std::endl;
	//std::cout << "# of materials : " << materials.size() << std::endl;
	
	std::vector<TriMesh *> res;
	for (tinyobj::shape_t s : shapes) {
		TriMesh* mesh = new TriMesh(_autorelease);
		
		for(unsigned long int v = 0; v < s.mesh.positions.size(); v += 3){
			mesh->vertices.push_back(Vertex(
				s.mesh.positions[v],
				s.mesh.positions[v+1],
				s.mesh.positions[v+2],
				1,1,1,1
			));
		}
		for(auto i : s.mesh.indices){
			mesh->indices.push_back(i);
		}
		for(unsigned long int v = 0; v < s.mesh.normals.size(); v += 3){
			mesh->setNormal(v/3,
				s.mesh.normals[v],
				s.mesh.normals[v+1],
				s.mesh.normals[v+2]
			);
		}
		for(unsigned long int v = 0; v < s.mesh.texcoords.size(); v += 2){
			mesh->setUV(v/2,
				s.mesh.texcoords[v],
				1-s.mesh.texcoords[v+1]
			);
		}

		res.push_back(mesh);
	}

	/*for (size_t i = 0; i < materials.size(); i++) {
		printf("material[%ld].name = %s\n", i, materials[i].name.c_str());
		printf("  material.Ka = (%f, %f ,%f)\n", materials[i].ambient[0], materials[i].ambient[1], materials[i].ambient[2]);
		printf("  material.Kd = (%f, %f ,%f)\n", materials[i].diffuse[0], materials[i].diffuse[1], materials[i].diffuse[2]);
		printf("  material.Ks = (%f, %f ,%f)\n", materials[i].specular[0], materials[i].specular[1], materials[i].specular[2]);
		printf("  material.Tr = (%f, %f ,%f)\n", materials[i].transmittance[0], materials[i].transmittance[1], materials[i].transmittance[2]);
		printf("  material.Ke = (%f, %f ,%f)\n", materials[i].emission[0], materials[i].emission[1], materials[i].emission[2]);
		printf("  material.Ns = %f\n", materials[i].shininess);
		printf("  material.Ni = %f\n", materials[i].ior);
		printf("  material.dissolve = %f\n", materials[i].dissolve);
		printf("  material.illum = %d\n", materials[i].illum);
		printf("  material.map_Ka = %s\n", materials[i].ambient_texname.c_str());
		printf("  material.map_Kd = %s\n", materials[i].diffuse_texname.c_str());
		printf("  material.map_Ks = %s\n", materials[i].specular_texname.c_str());
		printf("  material.map_Ns = %s\n", materials[i].normal_texname.c_str());
		std::map<std::string, std::string>::const_iterator it(materials[i].unknown_parameter.begin());
		std::map<std::string, std::string>::const_iterator itEnd(materials[i].unknown_parameter.end());
		for (; it != itEnd; it++) {
			printf("  material.%s = %s\n", it->first.c_str(), it->second.c_str());
		}
		printf("\n");
	}*/

	return res;
}