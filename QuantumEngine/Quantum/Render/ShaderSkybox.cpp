#include "stdafx.h"
#include "ShaderSkybox.h"

#include "Graphics.h"

#include "resource.h"



void loadResource(std::string& out, UINT id);

ShaderSkybox::ShaderSkybox()
{
	m_parameters.clear();

	m_parameters.push_back({ "textureSkybox",   0, TYPE_TEXTURE_2D,      UPDATE_PER_MATERIAL });
	m_parameters.push_back({ "cb_frameData",    2, TYPE_CONSTANT_BUFFER, UPDATE_PER_FRAME });
	m_parameters.push_back({ 0,                 0, TYPE_UNKNOWN,         UPDATE_UNKNOWN });
}

void ShaderSkybox::init()
{
	ID3D12Device* device = Graphics::getInstance().getDevice();

	// Load shader source and compile.
	std::string source;
	loadResource(source, ID_SHADER_SKYBOX);

	this->compileShaderSource<SHADER_VS>(source.data(), source.length());
	this->compileShaderSource<SHADER_PS>(source.data(), source.length());
	this->compile();

	// Pipeline state.
	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	this->getDefaultPipelineState(psoDesc);

	// Override input layout and depth function.
	psoDesc.InputLayout = { inputLayout, sizeof(inputLayout) / sizeof(inputLayout[0]) };
	psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pso));
}
