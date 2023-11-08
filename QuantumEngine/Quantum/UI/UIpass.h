#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "ShaderUI.h"
class D3D12Texture;
class QuWidget;
class QuWidgetText;


class UIpass
{

public:
	void init();
	void destroy();

	void setTexture(D3D12Texture* tex);

	void renderRectangles(ID3D12GraphicsCommandList* cmdList, const std::vector<QuWidget*>& list, UINT ObjectBase, UINT matrix);
	void renderText(ID3D12GraphicsCommandList* cmdList, const std::vector<QuWidgetText*>& list, UINT ObjectBase, UINT matrix, UINT texture);


private:
	VertexBuffer m_boxVB;
	IndexBuffer m_boxIB;

	ShaderUI m_shader;

	D3D12Texture* m_texture;
};

