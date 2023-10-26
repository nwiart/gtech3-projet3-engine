#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"

#include "TestConstantBuffer.h"
#include "D3D12ConstantBuffer.h"
#include "D3D12Texture.h"

#include "D3D12ResourceTransferUtility.h"

#include "InputCallback.h"

class Timer;
class Window;

class QuEntityLightDirectional;

typedef struct HWND__* HWND;



class Graphics : public InputCallback
{
public:

	void UpdateDirectionalLight(QuEntityLightDirectional*Entity);

	static inline Graphics& getInstance() { static Graphics g_graphics; return g_graphics; }

	static int initialize(Window* w);

	static void shutdown();


public:

	void initTestApp(UINT shaderResID);

	void update(const Timer& timer);
	void renderFrame(const Timer& timer);

	void flushCommandQueue();

	void resizeBuffers(int width, int height);



	/// Frame rendering steps.
public:

	void beginFrame();
	void endFrame();
	void swapBuffers();



	/// Initialization methods.
private:

	Graphics() { }

	int _init(Window* w);

	void RegisterInput();

	void createCommandList();
	void createSwapChain(HWND hwnd, int width, int height);
	void createCompatiblePSO(Shader* shader);

	void _shutdown();
	


private:

	void executePendingTransfers();


public:

	inline ID3D12Device* getDevice() const { return d3dDevice; }
	inline ID3D12GraphicsCommandList* getCommandList() const { return d3dCommandList; }
	inline ID3D12CommandAllocator* getCommandAllocator() const { return d3dCommandAllocator; }
	inline ID3D12CommandQueue* getCommandQueue() const { return d3dCommandQueue; }

	inline ID3D12Resource* getCurrentBackBuffer() const { return m_backBuffers[m_currentBackBuffer]; }
	inline D3D12_CPU_DESCRIPTOR_HANDLE getCurrentBackBufferView() const { return m_backBufferViews[m_currentBackBuffer]; }

	inline ID3D12DescriptorHeap* getShaderVisibleCBVHeap() const { return m_cbvHeap; }

	inline UINT getCBVDescriptorSize() const { return cbvDescriptorSize; }

	inline D3D12ResourceTransferUtility& getResourceTransferUtility() { return m_resourceTransferUtility; }



private:

	QuEntityLightDirectional* LightEntity;

	static const int NUM_BACK_BUFFERS = 2;

	

	D3D12ResourceTransferUtility m_resourceTransferUtility;

	HANDLE m_fenceEvent;
	IDXGIFactory1* dxgiFactory;
	ID3D12Device* d3dDevice;
	ID3D12Fence* d3dFence;
	ID3D12CommandQueue* d3dCommandQueue;
	ID3D12CommandAllocator* d3dCommandAllocator;
	ID3D12GraphicsCommandList* d3dCommandList;
	IDXGISwapChain* dxgiSwapChain;

	ID3D12DescriptorHeap* m_rtvHeap;
	ID3D12DescriptorHeap* m_dsvHeap;
	ID3D12DescriptorHeap* m_cbvHeap;      // Shader-accessible CBV-SRV-UAV heap, in which we copy the descriptors we need.
	ID3D12DescriptorHeap* m_samplerHeap;  // Shader-accessible Sampler heap, in which we copy the descriptors we need.

	ID3D12Resource* m_backBuffers[NUM_BACK_BUFFERS];
	ID3D12Resource* m_depthBuffer;
	D3D12_CPU_DESCRIPTOR_HANDLE m_backBufferViews[NUM_BACK_BUFFERS];
	D3D12_CPU_DESCRIPTOR_HANDLE m_depthBufferView;

	DXGI_FORMAT m_backBufferFormat;
	DXGI_FORMAT m_depthBufferFormat;

	UINT64 m_currentFenceValue;

	ID3D12PipelineState* m_defaultPSO;

#ifdef _DEBUG
	ID3D12Debug* d3dDebug;
#endif

	UINT rtvDescriptorSize, dsvDescriptorSize, cbvDescriptorSize, samplerDescriptorSize;

	int m_renderWidth;
	int m_renderHeight;

	int m_currentBackBuffer;



	/// Cube test.
public:

	int cursorX;
	int cursorY;

	float cameraX = 0;
	float cameraY = 0;
	float cameraZ = 0;
	float cameraW = 0;

	Shader m_shader;
	VertexBuffer m_vb;
	IndexBuffer m_ib;

	D3D12ConstantBuffer<TestConstantBuffer> m_cbFrameData;
	D3D12ConstantBuffer<ObjectConstantBuffer> m_cbObjectData[3];

	D3D12Texture m_texture;
};
