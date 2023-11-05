#pragma once

class D3D12Texture;



class Texture2D
{
public:

	Texture2D(const char* filePath);
	~Texture2D();

	inline D3D12Texture* getTexture() const { return m_texture; }


private:

	D3D12Texture* m_texture;
};
