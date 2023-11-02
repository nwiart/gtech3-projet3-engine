#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"

#include "TestConstantBuffer.h"
#include "D3D12ConstantBuffer.h"
#include "D3D12Texture.h"

#include "D3D12ResourceTransferUtility.h"

#include "Model.h"

#include "SkyboxRenderer.h"
#include <dxgi.h>



class Timer;
class Window;

class QuEntityLightDirectional;

typedef struct HWND__* HWND;



class Graphics
{
public:

	void UpdateDirectionalLight(QuEntityLightDirectional*Entity);
	void UpdateSkybox(D3D12Texture* tex);

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
	void OnKeyDown(WPARAM wparam);


	/// Initialization methods.
private:

	Graphics() { }

	int _init(Window* w);
	void CameraFollow();
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

#ifdef _DEBUG
	ID3D12Debug* d3dDebug;
#endif

	UINT rtvDescriptorSize, dsvDescriptorSize, cbvDescriptorSize, samplerDescriptorSize;
	int m_currentBackBuffer;



	/// Cube test.
public:

	int m_renderWidth;
	int m_renderHeight;

	int cursorX;
	int cursorY;

	int mouseLastStateX;
	int mouseLastStateY;

	float camYaw = 0.0f;
	float camPitch = 0.0f;

	float cameraX = 0;
	float cameraY = 0;
	float cameraZ = -4.F;
	float cameraW = 1.F;

	Shader m_shader;
	VertexBuffer m_vb;
	IndexBuffer m_ib;

	D3D12ConstantBuffer<TestConstantBuffer> m_cbFrameData;
	D3D12ConstantBuffer<ObjectConstantBuffer> m_cbObjectData;

	D3D12Texture m_texture;

	Model* m_sphere;

	struct RenderModel
	{
		Model* model;
		int cbID;
	};

	void addRenderModel(Model* model, DirectX::FXMMATRIX worldMatrix);

	std::vector<RenderModel> renderList;

	void freeRenderModel();


private:

	int nEntries = 0;

	SkyboxRenderer m_skyboxRenderer;

	D3D12Texture m_skyboxTexture;
};
