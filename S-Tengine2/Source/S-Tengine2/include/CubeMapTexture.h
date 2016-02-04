#pragma once

#include <Texture.h>

class CubeMapTexture : public Texture{
public:
	std::string faceSrc[6];
	unsigned char * faceData[6];

	// 0-6, used in the format GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceId in order to get the right face

	// _srxPrefix is the directory which leads to the cubemap (i.e. if the textures are "cubemap/posx.png", "cubemap/posy.png", etc. the _srcPrefix would be "cubemap")
	// all textures are assumed to be .png for now
	CubeMapTexture(std::string _srcPrefix, bool _storeData, bool _autoRelease, bool _useMipmaps = false);

	virtual void bufferData() const override;
	virtual void bufferDataFirst() const override;

	virtual void loadImageData();
	virtual void freeImageData();
};