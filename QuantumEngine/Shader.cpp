#include "stdafx.h"
#include "Shader.h"

#include "Graphics.h"

#include "TestConstantBuffer.h"

#include <iostream>



#define ROUND_256(x) ((x + 255) & ~255)



static void getOffset(INT* outRangeOffset, INT* outTableParam, ShaderParameter* rootParams, int shaderRegister, ShaderParameterType paramType)
{
	UINT numDescsInRange = 0;
	UINT numDescsTotal = 0;

	ShaderParameterType type = rootParams[0].type;
	ShaderParameterUpdate update = rootParams[0].updatePolicy;
	int baseSlot = rootParams[0].slot;

	while (rootParams->type != TYPE_UNKNOWN) {

		// Found it.
		if (rootParams->slot == shaderRegister && rootParams->type == paramType) {
			*outRangeOffset = numDescsTotal;
			*outTableParam = rootParams->updatePolicy;
			return;
		}

		rootParams++;
		numDescsInRange++;

		// Need a new range.
		if (rootParams->slot != baseSlot + numDescsInRange || type != rootParams->type || update != rootParams->updatePolicy) {

			numDescsInRange = 0;
			type = rootParams->type;
			baseSlot = rootParams->slot;

			update = rootParams->updatePolicy;
		}

		if (rootParams->type == TYPE_UNKNOWN) break;

		numDescsTotal++;
	}

	*outRangeOffset = -1;
	*outTableParam = -1;
}

static void generateSerializedRootSignature(ShaderParameter* rootParams, ID3DBlob** outRootSig)
{
	std::vector<D3D12_ROOT_PARAMETER> params;
	std::vector<D3D12_DESCRIPTOR_RANGE> descriptorTables[UPDATE_NUM_UPDATES];
	UINT numDescsInRange = 0;
	UINT baseRange = 0;

	for (int i = 0; i < UPDATE_NUM_UPDATES; ++i) {
		descriptorTables[i].reserve(64);
	}
	params.reserve(64);

	ShaderParameterType type = rootParams[0].type;
	ShaderParameterUpdate update = rootParams[0].updatePolicy;
	int baseSlot = rootParams[0].slot;

	while (true) {

		rootParams++;
		numDescsInRange++;

		// Need a new range.
		if (rootParams->slot != baseSlot + numDescsInRange || type != rootParams->type || update != rootParams->updatePolicy) {

			D3D12_DESCRIPTOR_RANGE_TYPE rangeType;
			switch (type) {
			case TYPE_CONSTANT_BUFFER: rangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV; break;
			case TYPE_TEXTURE_2D: rangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; break;
			}

			CD3DX12_DESCRIPTOR_RANGE r; r.Init(rangeType, numDescsInRange, baseSlot, numDescsInRange - 1);
			descriptorTables[update].push_back(r);

			numDescsInRange = 0;
			type = rootParams->type;
			baseSlot = rootParams->slot;

			// Need a new descriptor table.
			if (update != rootParams->updatePolicy) {
				CD3DX12_ROOT_PARAMETER p; p.InitAsDescriptorTable(descriptorTables[update].size(), descriptorTables[update].data());
				params.push_back(p);
			}

			update = rootParams->updatePolicy;
		}

		if (rootParams->type == TYPE_UNKNOWN) break;
	}

	// Create the signature description.
	CD3DX12_ROOT_SIGNATURE_DESC desc(params.size(), params.data(), 0, 0, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	CD3DX12_STATIC_SAMPLER_DESC ssDesc; ssDesc.Init(0);
	desc.NumStaticSamplers = 1;
	desc.pStaticSamplers = &ssDesc;

	D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, outRootSig, 0);
}



Shader::Shader()
	: m_serializedRootSig(0), m_rootSignature(0)
{
	ZeroMemory(m_shaderBytecodes, SHADER_NUM_TYPES * sizeof(ID3DBlob*));

	m_parameters.push_back({ "cb_objectData",   0, TYPE_CONSTANT_BUFFER, UPDATE_PER_INSTANCE });
	m_parameters.push_back({ "cb_materialData", 1, TYPE_CONSTANT_BUFFER, UPDATE_PER_MATERIAL });
	m_parameters.push_back({ "textureDiffuse",  0, TYPE_TEXTURE_2D,      UPDATE_PER_MATERIAL });
	m_parameters.push_back({ "cb_frameData",    2, TYPE_CONSTANT_BUFFER, UPDATE_PER_FRAME });
	m_parameters.push_back({ 0,                 0, TYPE_UNKNOWN,         UPDATE_UNKNOWN });
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
	ID3D12Device* device = Graphics::getInstance().getDevice();

	generateSerializedRootSignature(m_parameters.data(), &m_serializedRootSig);

	device->CreateRootSignature(0, m_serializedRootSig->GetBufferPointer(), m_serializedRootSig->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));

	return true;
}

bool Shader::isReady() const
{
	return m_shaderBytecodes[SHADER_VS] && m_shaderBytecodes[SHADER_PS] && m_rootSignature;
}

void Shader::setConstantBuffer(int shaderRegister, int offset)
{
	ID3D12Device* device = Graphics::getInstance().getDevice();
	ID3D12DescriptorHeap* cbvHeap = Graphics::getInstance().getShaderVisibleCBVHeap();
	ID3D12GraphicsCommandList* cmdList = Graphics::getInstance().getCommandList();
	UINT cbvDescriptorSize = Graphics::getInstance().getCBVDescriptorSize();

	INT descriptorOffset, tableIndex;
	getOffset(&descriptorOffset, &tableIndex, m_parameters.data(), shaderRegister, TYPE_CONSTANT_BUFFER);

	cmdList->SetGraphicsRootDescriptorTable(
		tableIndex,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(cbvHeap->GetGPUDescriptorHandleForHeapStart(), offset, cbvDescriptorSize));
}

void Shader::setTexture2D(int shaderRegister, int offset)
{
	ID3D12Device* device = Graphics::getInstance().getDevice();
	ID3D12DescriptorHeap* cbvHeap = Graphics::getInstance().getShaderVisibleCBVHeap();
	ID3D12GraphicsCommandList* cmdList = Graphics::getInstance().getCommandList();
	UINT cbvDescriptorSize = Graphics::getInstance().getCBVDescriptorSize();

	INT descriptorOffset, tableIndex;
	getOffset(&descriptorOffset, &tableIndex, m_parameters.data(), shaderRegister, TYPE_TEXTURE_2D);

	cmdList->SetGraphicsRootDescriptorTable(
		tableIndex,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(cbvHeap->GetGPUDescriptorHandleForHeapStart(), offset, cbvDescriptorSize));
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
