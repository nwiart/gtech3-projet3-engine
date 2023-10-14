#include "stdafx.h"
#include "D3D12UploadHelper.h"



void D3D12UploadHelper::Upload(ID3D12Resource* dest, const void* data, SIZE_T sizeInBytes)
{
	D3D12_RANGE r = { 0, 0 };
	void* mappedBuffer;
	dest->Map(0, &r, &mappedBuffer);

	memcpy(mappedBuffer, data, sizeInBytes);

	dest->Unmap(0, 0);
}
