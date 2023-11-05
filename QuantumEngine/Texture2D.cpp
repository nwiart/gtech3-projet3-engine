#include "Texture2D.h"

#include "stdafx.h"
#include "D3D12Texture.h"



Texture2D::Texture2D(const char* filePath)
{
	m_texture = new D3D12Texture();
	m_texture->loadFromDisk(filePath, D3D12_SRV_DIMENSION_TEXTURE2D);
}

Texture2D::~Texture2D()
{
	if (m_texture) {
		m_texture->destroy();
		delete m_texture;
	}
}
