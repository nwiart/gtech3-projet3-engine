#pragma once



class D3D12Texture
{
public:

	D3D12Texture();

	void create();

	void destroy();

	inline const D3D12_CPU_DESCRIPTOR_HANDLE& getShaderResourceView() const { return m_shaderResourceView; }


private:

	ID3D12Resource* m_resource;
	ID3D12DescriptorHeap* m_srvHeap;

	D3D12_CPU_DESCRIPTOR_HANDLE m_shaderResourceView;
};
