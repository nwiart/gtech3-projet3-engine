#pragma once

#include <d3d12.h>

#include <vector>



class D3D12ResourceTransferUtility
{
public:

	D3D12ResourceTransferUtility() { }

	void init(ID3D12CommandQueue* cmdQueue, ID3D12CommandAllocator* cmdAlloc, ID3D12GraphicsCommandList* cmdList);

	void copy(ID3D12Resource* intermediate, ID3D12Resource* dest);


	bool performTransfers();
	void clear();


private:

	struct Transfer
	{
		ID3D12Resource* m_intermediate;
		ID3D12Resource* m_destination;
	};

	ID3D12CommandQueue* m_commandQueue;
	ID3D12CommandAllocator* m_commandAllocator;
	ID3D12GraphicsCommandList* m_commandList;

	std::vector<Transfer> m_queuedTransfers;
};
