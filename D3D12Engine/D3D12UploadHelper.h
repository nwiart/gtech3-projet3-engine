#pragma once

#include <d3d12.h>



class D3D12UploadHelper
{
public:

		/// Upload data to a resource stored in the upload heap.
	static void Upload(ID3D12Resource* dest, const void* data, SIZE_T sizeInBytes);
};
