#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "D3D12Texture.h"

#include "ShaderSkybox.h"



class SkyboxRenderer
{
public:

	void init();
	void destroy();

	void setTexture(D3D12Texture* tex);

	void render(ID3D12GraphicsCommandList* cmdList);


	
private:

	VertexBuffer m_boxVB;
	IndexBuffer m_boxIB;

	ShaderSkybox m_shader;

	D3D12Texture m_skyboxTexture;
	D3D12Texture* m_textureCube;
};
