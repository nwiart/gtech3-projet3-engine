#include "stdafx.h"
#include "Shader.h"

#include "Game.h"

#include "TestConstantBuffer.h"

#include <iostream>



#define ROUND_256(x) ((x + 255) & ~255)

Shader::Shader()
	: m_rootSignature(0)
{
	ZeroMemory(m_shaderBytecodes, SHADER_NUM_TYPES * sizeof(ID3DBlob*));
}

Shader::~Shader()
{

}

void Shader::destroy()
{
	if (m_rootSignature) {
		m_rootSignature->Release();

		m_shaderBytecodes[SHADER_VS]->Release();
		m_shaderBytecodes[SHADER_PS]->Release();
	}
}

bool Shader::compile()
{
	ID3D12Device* device = Game::getInstance()->getDevice();

	// Our only parameter is a Descriptor table containing one Constant buffer view.
	CD3DX12_DESCRIPTOR_RANGE table(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 2, 0);
	CD3DX12_ROOT_PARAMETER params[1];
	params[0].InitAsDescriptorTable(1, &table);

	// Create the signature description.
	CD3DX12_ROOT_SIGNATURE_DESC desc(1, params, 0, 0, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	
	// Create root signature.
	D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &m_serializedRootSig, 0);

	device->CreateRootSignature(0, m_serializedRootSig->GetBufferPointer(), m_serializedRootSig->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));

	return true;
}

bool Shader::isReady() const
{
	return m_shaderBytecodes[SHADER_VS] && m_shaderBytecodes[SHADER_PS] && m_rootSignature;
}

void Shader::setShaderSource(Type shaderType, const char* source, SIZE_T size)
{
	if (m_shaderBytecodes[shaderType]) return;

	static const char* entryPoints[] =
	{
		"vs_main",
		"ps_main",
	};
	static const char* versions[] =
	{
		"vs_5_0",
		"ps_5_0",
	};

	ID3DBlob* errors = 0;
	D3DCompile(source, size, 0, 0, 0, entryPoints[shaderType], versions[shaderType], D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &m_shaderBytecodes[shaderType], &errors);

	if (errors) {
		std::cout << reinterpret_cast<const char*>(errors->GetBufferPointer()) << '\n';
		errors->Release();
	}
}
