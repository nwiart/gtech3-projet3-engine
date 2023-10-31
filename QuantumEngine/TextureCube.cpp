#include "TextureCube.h"

#include "stdafx.h"
#include "D3D12Texture.h"



TextureCube::TextureCube(const char* filePath)
{
	m_texture = new D3D12Texture();
	m_texture->loadFromDisk(filePath, D3D12_SRV_DIMENSION_TEXTURECUBE);
}

TextureCube::~TextureCube()
{
	m_texture->destroy();
	delete m_texture;
}
