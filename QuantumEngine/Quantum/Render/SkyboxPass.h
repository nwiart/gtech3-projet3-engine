#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "ShaderSkybox.h"

class D3D12Texture;



class SkyboxPass
{
public:

	void init();
	void destroy();

	D3D12Texture* getTexture() const { return m_textureCube; }
	void setTexture(D3D12Texture* tex);

	void render(ID3D12GraphicsCommandList* cmdList, UINT frameData);


	
private:

	VertexBuffer m_boxVB;
	IndexBuffer m_boxIB;

	ShaderSkybox m_shader;

	D3D12Texture* m_textureCube;
};
