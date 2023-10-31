#pragma once


class D3D12Texture;



class TextureCube
{
public:

	TextureCube(const char* filePath);
	~TextureCube();

	inline D3D12Texture* getTexture() const { return m_texture; }


private:

	D3D12Texture* m_texture;
};
