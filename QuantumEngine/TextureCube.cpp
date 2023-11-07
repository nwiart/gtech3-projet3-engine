#include "TextureCube.h"

#include "stdafx.h"
#include "D3D12Texture.h"



TextureCube::TextureCube(const char* filePath)
{
	D3D12Texture* tex = new D3D12Texture();
	tex->loadFromDisk(filePath, D3D12_SRV_DIMENSION_TEXTURECUBE);

	this->setTexture(tex);
}
