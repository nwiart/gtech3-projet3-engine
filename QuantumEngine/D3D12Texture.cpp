#include "stdafx.h"
#include "D3D12Texture.h"

#include "Graphics.h"



D3D12Texture::D3D12Texture()
	: m_resource(0), m_srvHeap(0)
{

}

void D3D12Texture::create()
{
	ID3D12Device* device = Graphics::getInstance().getDevice();

	DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UINT;

	// Create texture resource.
	// TODO : This is in the upload heap!!! Move to default heap using the transfer helper.
	device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Tex2D(format, 2, 2),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		0,
		IID_PPV_ARGS(&m_resource));

	// Dummy 2x2 black / magenta checker.
	UINT* data;
	m_resource->Map(0, 0, (void**) &data);

	data[0] = 0xFF000000;
	data[1] = 0xFFFF00FF;
	data[2] = 0xFFFF00FF;
	data[3] = 0xFF000000;

	m_resource->Unmap(0, 0);

	// CPU descriptor heap and view.
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = { };
	srvHeapDesc.NumDescriptors = 1;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_srvHeap));

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = { };
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0F;
	device->CreateShaderResourceView(m_resource, &srvDesc, m_srvHeap->GetCPUDescriptorHandleForHeapStart());

	m_shaderResourceView = m_srvHeap->GetCPUDescriptorHandleForHeapStart();
}

void D3D12Texture::destroy()
{
	if (m_resource) {
		m_resource->Release();
		m_resource = 0;

		m_srvHeap->Release();
		m_srvHeap = 0;
	}
}
