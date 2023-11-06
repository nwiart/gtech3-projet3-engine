#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "ShaderUI.h"
class D3D12Texture;
class QuWidget;


class UIpass
{

public:
	void init();
	void destroy();

	void setTexture(D3D12Texture* tex);

	void render(ID3D12GraphicsCommandList* cmdList, const std::vector<QuWidget*>& list, UINT ObjectBase, UINT matrix);



private:
	VertexBuffer m_boxVB;
	IndexBuffer m_boxIB;

	ShaderUI m_shader;

	D3D12Texture* m_texture;
};

