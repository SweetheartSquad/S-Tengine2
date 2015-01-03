#pragma once

#include <string>
#include "Box2DLevel.h"

class VoxelJoint;
class VoxelMesh;
class TriMesh;

class Resource
{
private:
	Resource();
public:
	~Resource();

	/**
	* Readable Image Formats:
	* BMP - non-1bpp, non-RLE (from stb_image documentation)
	* PNG - non-interlaced (from stb_image documentation)
	* JPG - JPEG baseline (from stb_image documentation)
	* TGA - greyscale or RGB or RGBA or indexed, uncompressed or RLE
	* DDS - DXT1/2/3/4/5, uncompressed, cubemaps (can't read 3D DDS files yet)
	* PSD - (from stb_image documentation)
	* HDR - converted to LDR, unless loaded with *HDR* functions (RGBE or RGBdivA or RGBdivA2)
	* 
	* @param _src The path to the image file
	* @param _width The width of the image 
	* @param _hright The height of the image
	* @param _SOILLoadMode the load mode of soil. ed SOIL_AUTO
	* @param _channels Stores the returned number of channels in the image
	* @return The image data 
	*/
	static unsigned char* loadImage(const char* _src,  int _width, int _height, int _SOILLoadMode, int * _channels);

	/**
	* Free image data memory 
	* @param _image The image to free
	*/
	static void freeImageData(unsigned char* _image);

	/**
	* Used to load a tri mesh from an obj file. This loader
	* supports reading vertex position, normals and uvs
	* 
	* This loader is sort of slow at the moment. Hopefully
	* it will get better with further optimization
	*
	* @param _objSrc The path to the obj file
	* @returns The loaded TriMesh
	*/
	static TriMesh* loadMeshFromObj(std::string _objSrc);

	/**
	* Used to load a VoxelJoint from a vox model JSON file
	* This will load all voxel points and animations 
	* withthe proper hirearchy
	* 
	* @param The src path to the json file
	* @returns The loaded VoxelJoint
	*/
	static VoxelJoint * loadVoxelModel(std::string _jsonSrc); 

	static Box2DLevel * loadFizzXLevel(std::string _jsonSrc, std::string _imageLocation);
};