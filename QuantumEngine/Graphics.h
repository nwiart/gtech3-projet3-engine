#pragma once

#include "D3D12ResourceTransferUtility.h"

#include "Quantum/Render/RenderScene.h"
#include "Quantum/UI/UIrenderer.h"



class Timer;
class Window;

class Model;

typedef struct HWND__* HWND;



class Graphics
{
public:

	static inline Graphics& getInstance() { static Graphics g_graphics; return g_graphics; }

	static int initialize(Window* w);

	static void shutdown();


public:

	void addRenderModel(class QuEntityRenderModel* model) { m_renderScene.addRenderModel(model); }
	void addParticleEmitter(class QuEntityParticleEmitter* pe) { m_renderScene.addParticleEmitter(pe); }
	void setCamera(class QuEntityCamera* camera) { m_renderScene.setCamera(camera); }
	void setDirectionalLight(class QuEntityLightDirectional* dl) { m_renderScene.setDirectionalLight(dl); }
	void addPointLight(class QuEntityLightPoint* pl) { m_renderScene.addPointLight(pl); }
	void setSkybox(class QuEntityRenderSkybox* skybox) { m_renderScene.setSkybox(skybox); }


	void renderFrame();

	void flushCommandQueue();

	void resizeBuffers(int width, int height);

	UINT allocateDescriptorTable(INT numDescriptors);
	void setGlobalDescriptor(UINT index, D3D12_CPU_DESCRIPTOR_HANDLE descriptor);



	/// Frame rendering steps.
public:

	void beginFrame();
	void endFrame();
	void swapBuffers();



	/// Initialization methods.
private:

	Graphics() { }

	int _init(Window* w);

	void createCommandList();
	void createSwapChain(HWND hwnd, int width, int height);

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

	inline const RenderScene& getRenderScene() const { return m_renderScene; }

	inline int getRenderWidth() const { return m_renderWidth; }
	inline int getRenderHeight() const { return m_renderHeight; }

	inline UIRenderer* getUIRenderer() { return m_renderUI; }




private:

	static const int NUM_BACK_BUFFERS = 2;

	int m_renderWidth;
	int m_renderHeight;

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

#ifdef _DEBUG
	ID3D12Debug* d3dDebug;
#endif

	UINT rtvDescriptorSize, dsvDescriptorSize, cbvDescriptorSize, samplerDescriptorSize;
	int m_currentBackBuffer;



private:

	int nEntries = 0;

	RenderScene m_renderScene;

	UIRenderer* m_renderUI;
};
