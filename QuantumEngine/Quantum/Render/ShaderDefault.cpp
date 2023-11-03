#include "stdafx.h"
#include "ShaderDefault.h"

#include "Graphics.h"

#include "resource.h"



void loadResource(std::string& out, UINT id);

ShaderDefault::ShaderDefault()
{
	m_parameters.push_back({ "cb_objectData",   0, TYPE_CONSTANT_BUFFER, UPDATE_PER_INSTANCE });
	m_parameters.push_back({ "cb_materialData", 1, TYPE_CONSTANT_BUFFER, UPDATE_PER_MATERIAL });
	m_parameters.push_back({ "textureDiffuse",  0, TYPE_TEXTURE_2D,      UPDATE_PER_MATERIAL });
	m_parameters.push_back({ "cb_frameData",    2, TYPE_CONSTANT_BUFFER, UPDATE_PER_FRAME });
	m_parameters.push_back({ 0,                 0, TYPE_UNKNOWN,         UPDATE_UNKNOWN });
}

void ShaderDefault::init()
{
	ID3D12Device* device = Graphics::getInstance().getDevice();

	// Load shader source and compile.
	std::string source;
	loadResource(source, ID_SHADER_DEFAULT);

	this->compileShaderSource<SHADER_VS>(source.data(), source.length());
	this->compileShaderSource<SHADER_PS>(source.data(), source.length());
	this->compile();

	// Pipeline state.
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	this->getDefaultPipelineState(psoDesc);

	device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pso));
}
