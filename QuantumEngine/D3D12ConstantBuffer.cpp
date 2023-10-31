#include "stdafx.h"
#include "D3D12ConstantBuffer.h"

#include "Graphics.h"



#define ROUND_256(x) ((x + 255) & ~255)



D3D12ConstantBufferBase::D3D12ConstantBufferBase()
	: m_cbResource(0), m_cbvHeap(0)
{

}

void D3D12ConstantBufferBase::init(SIZE_T elemSize, SIZE_T numElems)
{
	// Round size to higher multiple of 256 (for satisfying hardware requirements).
	m_cbSize = ROUND_256(elemSize);

	ID3D12Device* device = Graphics::getInstance().getDevice();

	// Create CBV heap.
	D3D12_DESCRIPTOR_HEAP_DESC cbvDesc;
	cbvDesc.NumDescriptors = numElems;
	cbvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;   // NOT shader visible! The needed descriptors will be copied to a special heap for shader usage.
	cbvDesc.NodeMask = 0;
	device->CreateDescriptorHeap(&cbvDesc, IID_PPV_ARGS(&m_cbvHeap));

	// Create resource.
	device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(m_cbSize * numElems), D3D12_RESOURCE_STATE_GENERIC_READ, 0, IID_PPV_ARGS(&m_cbResource));

	// Create a constant buffer view.
	m_cbView = m_cbvHeap->GetCPUDescriptorHandleForHeapStart();
	for (int i = 0; i < numElems; i++) 
	{
		D3D12_CONSTANT_BUFFER_VIEW_DESC desc;
		desc.BufferLocation = m_cbResource->GetGPUVirtualAddress() + m_cbSize * i;
		desc.SizeInBytes = m_cbSize;
		device->CreateConstantBufferView(&desc, m_cbView);
		m_cbView.ptr += Graphics::getInstance().getCBVDescriptorSize();
	}

}

void D3D12ConstantBufferBase::destroy()
{
	if (m_cbResource) m_cbResource->Release();
	if (m_cbvHeap) m_cbvHeap->Release();

	m_cbResource = 0;
	m_cbvHeap = 0;
}

void D3D12ConstantBufferBase::update(int elemIndex, const void* data, SIZE_T size)
{
	D3D12_RANGE range = { 0, 0 };
	char* bufferData;

	m_cbResource->Map(0, &range, (void**) &bufferData);

	memcpy(bufferData + elemIndex * m_cbSize, data, size);

	m_cbResource->Unmap(0, 0);
}

void D3D12ConstantBufferBase::updateRange(int elemIndexStart, int elemCount, const void* data, SIZE_T size)
{
	D3D12_RANGE range = { 0, 0 };
	char* bufferData;

	m_cbResource->Map(0, &range, (void**) &bufferData);

	memcpy(bufferData + elemIndexStart * m_cbSize, data, size);

	m_cbResource->Unmap(0, 0);
}

D3D12_CPU_DESCRIPTOR_HANDLE D3D12ConstantBufferBase::getDescriptor(int id)
{
	// TODO: insérer une instruction return ici
	D3D12_CPU_DESCRIPTOR_HANDLE temp = m_cbvHeap->GetCPUDescriptorHandleForHeapStart();
	temp.ptr += id * Graphics::getInstance().getCBVDescriptorSize();
	return temp;
}
