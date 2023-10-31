#pragma once

#include <d3d12.h>



class VertexBuffer
{
public:

	VertexBuffer();
	~VertexBuffer();

	void setData(const void* data, int byteSize, int stride);

	void destroy();

	inline ID3D12Resource* getResource() const { return m_buffer; }
	inline const D3D12_VERTEX_BUFFER_VIEW& getVertexBufferView() const { return m_bufferView; }
	


private:

	ID3D12Resource* m_buffer;
	D3D12_VERTEX_BUFFER_VIEW m_bufferView;
};
