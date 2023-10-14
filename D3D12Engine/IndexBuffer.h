#pragma once

#include <d3d12.h>



class IndexBuffer
{
public:

	IndexBuffer();
	~IndexBuffer();

	void setData(const void* data, int byteSize);

	void destroy();

	inline ID3D12Resource* getResource() const { return m_buffer; }
	inline const D3D12_INDEX_BUFFER_VIEW& getIndexBufferView() const { return m_bufferView; }



private:

	ID3D12Resource* m_buffer;
	D3D12_INDEX_BUFFER_VIEW m_bufferView;
};
