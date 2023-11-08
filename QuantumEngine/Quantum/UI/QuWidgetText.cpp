#include "stdafx.h"
#include "QuWidgetText.h"
#include "Graphics.h"
#include"VertexBuffer.h"

struct textVertex { float pos[3]; float uv[2]; };

QuWidgetText::QuWidgetText()
{
	m_vBuffer = new VertexBuffer();
}

QuWidgetText::~QuWidgetText()
{
	m_vBuffer->destroy();
}

void QuWidgetText::SetText(std::string text)
{
	m_text = text;
	if (m_vBuffer) m_vBuffer->destroy();
	std::vector<textVertex> m_data;
	for(char cara : m_text)
	{
		float X = m_data.size() * 32;
		float u = (cara % 16) / 16.0F;
		float v = (cara / 16) / 16.0F;
		m_data.push_back({ { X, 0, 0 }, { u, v } });
		m_data.push_back({ { X+32, 0, 0 }, { u + 1 / 16.0F, v } });
		m_data.push_back({ { X, 64, 0 }, { u, v + 1 / 16.0F} });
		m_data.push_back({ { X, 64, 0 }, { u, v + 1 / 16.0F} });
		m_data.push_back({ { X+32, 0, 0 }, { u + 1 / 16.0F, v } });
		m_data.push_back({ { X+32, 64, 0 }, { u + 1 / 16.0F, v + 1 / 16.0F } });

		numbOfVertices += 6;
	}
	m_vBuffer->setData(m_data.data(), m_data.size()*sizeof(textVertex), sizeof(textVertex));
}
