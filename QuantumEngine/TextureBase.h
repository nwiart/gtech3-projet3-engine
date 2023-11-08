#pragma once

class D3D12Texture;



class TextureBase
{
protected:

	TextureBase();
	~TextureBase();

	inline void setTexture(D3D12Texture* t) { m_texture = t; }

public:

	inline D3D12Texture* getTexture() const { return m_texture; }


private:

	D3D12Texture* m_texture;
};
