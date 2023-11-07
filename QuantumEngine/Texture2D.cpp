#include "Texture2D.h"

#include "stdafx.h"
#include "D3D12Texture.h"



Texture2D::Texture2D(const char* filePath)
{
	D3D12Texture* tex = new D3D12Texture();
	tex->loadFromDisk(filePath, D3D12_SRV_DIMENSION_TEXTURE2D);

	this->setTexture(tex);
}
