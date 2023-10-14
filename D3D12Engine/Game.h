#pragma once

#include <d3d12.h>
#include <dxgi.h>

#include "Window.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"

#include "TestConstantBuffer.h"
#include "D3D12ConstantBuffer.h"

#include "D3D12ResourceTransferUtility.h"

class Shader;



class Game
{
public:

	Game();

	int init(int windowWidth, int windowHeight);
	void shutdown();

	void mainLoop();

private:

	void update();

	void render();

	void createDepthBuffer(int width, int height);

	void createCompatiblePSO(Shader* shader);

	void flushCommandQueue();

public:

	inline ID3D12Device* getDevice() const { return d3dDevice; }
	inline ID3D12GraphicsCommandList* getCommandList() const { return d3dCommandList; }

	inline ID3D12Resource* getCurrentBackBuffer() const { return backBuffers[currentBackBuffer]; }
	inline D3D12_CPU_DESCRIPTOR_HANDLE getCurrentBackBufferView() const { return backBufferViews[currentBackBuffer]; }

	inline D3D12ResourceTransferUtility& getResourceTransferUtility() { return m_resourceTransferUtility; }

	static inline Game* getInstance() { return m_instance; }


public:
	
	static Game* m_instance;

	Window m_window;
	int m_windowWidth;
	int m_windowHeight;

	D3D12ResourceTransferUtility m_resourceTransferUtility;

	HANDLE m_fenceEvent;
	IDXGIFactory1* dxgiFactory;
	ID3D12Device* d3dDevice;
	ID3D12Fence* d3dFence;
	ID3D12CommandQueue* d3dCommandQueue;
	ID3D12CommandAllocator* d3dCommandAllocator;
	ID3D12GraphicsCommandList* d3dCommandList;
	IDXGISwapChain* dxgiSwapChain;
	ID3D12Debug* d3dDebug;
	UINT rtvDescriptorSize, dsvDescriptorSize, cbvDescriptorSize, samplerDescriptorSize;

	ID3D12DescriptorHeap* rtvHeap;
	ID3D12DescriptorHeap* dsvHeap;
	ComPtr<ID3D12DescriptorHeap> m_cbvHeap;      // Shader-accessible CBV-SRV-UAV heap, in which we copy the descriptors we need.
	ComPtr<ID3D12DescriptorHeap> m_samplerHeap;  // Shader-accessible Sampler heap, in which we copy the descriptors we need.

	ID3D12Resource* backBuffers[2];
	ID3D12Resource* depthBuffer;
	D3D12_CPU_DESCRIPTOR_HANDLE backBufferViews[2];
	D3D12_CPU_DESCRIPTOR_HANDLE depthBufferView;
	int currentBackBuffer;

	int m_numBackBuffers;
	DXGI_FORMAT m_backBufferFormat;
	DXGI_FORMAT m_depthBufferFormat;

	UINT64 m_currentFenceValue;

	ID3D12PipelineState* m_defaultPSO;



		/// Cube test.
	int cursorX;
	int cursorY;

	Shader m_shader;
	VertexBuffer m_vb;
	IndexBuffer m_ib;

	D3D12ConstantBuffer<TestConstantBuffer> m_cbFrameData;
	D3D12ConstantBuffer<ObjectConstantBuffer> m_cbObjectData;
};
