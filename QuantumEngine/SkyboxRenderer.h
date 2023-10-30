#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "ShaderSkybox.h"

class D3D12Texture;



class SkyboxRenderer
{
public:

	void init();

	void setTexture(D3D12Texture* tex);

	void render(ID3D12GraphicsCommandList* cmdList, int resourceID);


	
private:

	VertexBuffer m_boxVB;
	IndexBuffer m_boxIB;

	ShaderSkybox m_shader;

	D3D12Texture* m_textureCube;
};
