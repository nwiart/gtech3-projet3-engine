#include "stdafx.h"
#include "D3D12ResourceTransferUtility.h"



void D3D12ResourceTransferUtility::init(ID3D12CommandQueue* cmdQueue, ID3D12CommandAllocator* cmdAlloc, ID3D12GraphicsCommandList* cmdList)
{
	m_commandQueue = cmdQueue;
	m_commandAllocator = cmdAlloc;
	m_commandList = cmdList;

	// For good measure.
	m_queuedTransfers.reserve(128);
}

void D3D12ResourceTransferUtility::copy(ID3D12Resource* intermediate, ID3D12Resource* dest)
{
	intermediate->AddRef();

	Transfer t = { intermediate, dest };
	m_queuedTransfers.push_back(t);
}


bool D3D12ResourceTransferUtility::performTransfers()
{
	if (m_queuedTransfers.empty()) return false;

	m_commandAllocator->Reset();
	m_commandList->Reset(m_commandAllocator, 0);

	for (const Transfer& t : m_queuedTransfers)
	{
		m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(t.m_destination, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));

		m_commandList->CopyResource(t.m_destination, t.m_intermediate);

		m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(t.m_destination, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ));
	}


	m_commandList->Close();

	ID3D12CommandList* lists[] = { m_commandList };
	m_commandQueue->ExecuteCommandLists(1, lists);

	return true;
}

void D3D12ResourceTransferUtility::clear()
{
	for (const Transfer& t : m_queuedTransfers)
	{
		t.m_intermediate->Release();
	}

	m_queuedTransfers.clear();
}
