#pragma once

#include <d3d12.h>



	/// Wrapper class for simplifying creating and usage of general constant buffers.
	/// The template specialization class below accepts any structure as data buffer.
class D3D12ConstantBufferBase
{
protected:

	D3D12ConstantBufferBase();

	void init(SIZE_T elemSize, SIZE_T numElems);

	void destroy();

	void update(int elemIndex, const void* data, SIZE_T size);

	void updateRange(int elemIndexStart, int elemCount, const void* data, SIZE_T size);

public:

		/// Resource expose for use in rendering.
	inline ID3D12Resource* getResource() const { return m_cbResource; }
	inline ID3D12DescriptorHeap* getDescriptorHeap() const { return m_cbvHeap; }
	D3D12_CPU_DESCRIPTOR_HANDLE getDescriptor(int id = 0);


private:

	ID3D12Resource* m_cbResource;
	ID3D12DescriptorHeap* m_cbvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE m_cbView;

	UINT m_cbSize;
};



template<class T>
class D3D12ConstantBuffer : public D3D12ConstantBufferBase
{
public:

	D3D12ConstantBuffer();

	inline void init(SIZE_T numElems = 1);

	inline void destroy();

	inline void update(int elemIndex, const T& data);
	inline void updateRange(int elemIndexStart, int elemCount, const T* data);
};



#include "D3D12ConstantBuffer.inl"
