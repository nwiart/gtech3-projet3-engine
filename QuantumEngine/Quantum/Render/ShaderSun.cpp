#include "stdafx.h"
#include "ShaderSun.h"

#include "Graphics.h"

#include "resource.h"



void loadResource(std::string& out, UINT id);

ShaderSun::ShaderSun()
{
	m_parameters.push_back({ "cb_data", 2, TYPE_CONSTANT_BUFFER, UPDATE_PER_INSTANCE });
	m_parameters.push_back({ "cb_data", 1, TYPE_CONSTANT_BUFFER, UPDATE_PER_MATERIAL });
	m_parameters.push_back({ "cb_data", 0, TYPE_CONSTANT_BUFFER, UPDATE_PER_FRAME });
	m_parameters.push_back({ 0,         0, TYPE_UNKNOWN,         UPDATE_UNKNOWN });
}

void ShaderSun::init()
{
	ID3D12Device* device = Graphics::getInstance().getDevice();

	// Load shader source and compile.
	std::string source;
	loadResource(source, ID_SHADER_PP_SUN);

	this->compileShaderSource<SHADER_VS>(source.data(), source.length());
	this->compileShaderSource<SHADER_PS>(source.data(), source.length());
	this->compile();

	// Pipeline state.
	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	this->getDefaultPipelineState(psoDesc);

	// Override input layout and depth function.
	psoDesc.InputLayout = { inputLayout, sizeof(inputLayout) / sizeof(inputLayout[0]) };
	psoDesc.DepthStencilState.DepthEnable = false;
	psoDesc.BlendState.RenderTarget[0].BlendEnable = true;
	psoDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	psoDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	psoDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;

	device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pso));
}
