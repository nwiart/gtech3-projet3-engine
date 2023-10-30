#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "ShaderSkybox.h"



class SkyboxRenderer
{
public:

	void init();

	
private:

	VertexBuffer m_boxVB;
	IndexBuffer m_boxIB;

	ShaderSkybox m_shader;
};
