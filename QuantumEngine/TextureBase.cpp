#include "TextureBase.h"

#include "stdafx.h"
#include "D3D12Texture.h"



TextureBase::TextureBase()
	: m_texture(0)
{

}

TextureBase::~TextureBase()
{
	if (m_texture) {
		m_texture->destroy();
		delete m_texture;
	}
}
