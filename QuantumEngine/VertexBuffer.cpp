#include "stdafx.h"
#include "VertexBuffer.h"

#include "D3D12UploadHelper.h"

#include "Graphics.h"



VertexBuffer::VertexBuffer()
	: m_buffer(0)
{

}

VertexBuffer::~VertexBuffer()
{
	
}

void VertexBuffer::setData(const void* data, int byteSize)
{
	ID3D12Device* device = Graphics::getInstance().getDevice();

	// Create our default heap resource.
	device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(byteSize),
		D3D12_RESOURCE_STATE_COMMON,
		0,
		IID_PPV_ARGS(&m_buffer));

	// Create a resource in the upload heap and fill it with data.
	ID3D12Resource* uploadBuffer;
	device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(byteSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		0,
		IID_PPV_ARGS(&uploadBuffer));

	D3D12UploadHelper::Upload(uploadBuffer, data, byteSize);
	
	// Schedule transfer.
	Graphics::getInstance().getResourceTransferUtility().copy(uploadBuffer, m_buffer);
	uploadBuffer->Release();

	// Fill in the vertex buffer view.
	m_bufferView.BufferLocation = m_buffer->GetGPUVirtualAddress();
	m_bufferView.SizeInBytes = byteSize;
	m_bufferView.StrideInBytes = 6 * sizeof(float);
}

void VertexBuffer::destroy()
{
	if (m_buffer) {
		m_buffer->Release();
		m_buffer = 0;
	}
}
