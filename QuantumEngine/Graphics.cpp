#include "stdafx.h"
#include "Graphics.h"

#include "Timer.h"
#include "Window.h"

#include "QuEntityLightDirectional.h"
#include "InputSystem.h"

XMVECTOR DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
XMVECTOR DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
XMVECTOR DefaultUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
XMVECTOR camForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
XMVECTOR camRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
XMVECTOR camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);



// Loading resources from the executable.
void loadResource(std::string& out, UINT id)
{
	HINSTANCE hinstance = GetModuleHandle(0);

	HRSRC res = FindResource(hinstance, MAKEINTRESOURCE(id), RT_RCDATA);
	HGLOBAL resg = LoadResource(hinstance, res);
	SIZE_T size = SizeofResource(hinstance, res);

	out.assign(reinterpret_cast<const char*>(LockResource(resg)), size);

	FreeResource(resg);
}



//
// Public API.
//

int Graphics::initialize(Window* window)
{
	return Graphics::getInstance()._init(window);
}

void Graphics::shutdown()
{
	Graphics::getInstance()._shutdown();
}



//
// Initialization.
//

int Graphics::_init(Window* window)
{
#ifdef _DEBUG
	D3D12GetDebugInterface(IID_PPV_ARGS(&d3dDebug));
	d3dDebug->EnableDebugLayer();
#endif

	HRESULT result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
	if (FAILED(result)) {
		return 1;
	}

	// Device.
	result = D3D12CreateDevice(0, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&d3dDevice));
	if (FAILED(result)) {
		return 2;
	}

	// Fence.
	m_currentFenceValue = 0;
	d3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&d3dFence));

	m_fenceEvent = CreateEventEx(0, 0, 0, EVENT_ALL_ACCESS);

	// Descriptor sizes.
	rtvDescriptorSize = d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	dsvDescriptorSize = d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	cbvDescriptorSize = d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	samplerDescriptorSize = d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);

	// Command queue, allocator and list.
	this->createCommandList();

	// Swap chain.
	window->getClientSize(m_renderWidth, m_renderHeight);
	HWND hwnd = window->getHwnd();

	this->createSwapChain(hwnd, m_renderWidth, m_renderHeight);


	// Shader-bound descriptor heaps.
	D3D12_DESCRIPTOR_HEAP_DESC spDesc;
	spDesc.NumDescriptors = 65536;
	spDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	spDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	spDesc.NodeMask = 0;
	d3dDevice->CreateDescriptorHeap(&spDesc, IID_PPV_ARGS(&m_cbvHeap));

	spDesc.NumDescriptors = 256;
	spDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
	spDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	d3dDevice->CreateDescriptorHeap(&spDesc, IID_PPV_ARGS(&m_samplerHeap));

	m_renderScene.init();

	return 0;
}

void Graphics::createCommandList()
{
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = { };
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	d3dDevice->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&d3dCommandQueue));

	d3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&d3dCommandAllocator));

	d3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, d3dCommandAllocator, 0, IID_PPV_ARGS(&d3dCommandList));
	d3dCommandList->Close();

	m_resourceTransferUtility.init(d3dCommandQueue, d3dCommandAllocator, d3dCommandList);
}

void Graphics::createSwapChain(HWND hwnd, int width, int height)
{
	// Swap chain.
	m_backBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	DXGI_SWAP_CHAIN_DESC sd = { 0 };
	sd.BufferDesc.Format = m_backBufferFormat;
	sd.BufferDesc.Width = m_renderWidth;
	sd.BufferDesc.Height = m_renderHeight;
	sd.BufferCount = NUM_BACK_BUFFERS;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.SampleDesc.Quality = 0;
	sd.SampleDesc.Count = 1;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.OutputWindow = hwnd;
	sd.Windowed = true;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH | DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

	dxgiFactory->CreateSwapChain(d3dCommandQueue, &sd, &dxgiSwapChain);

	// Back buffers.
	m_currentBackBuffer = 0;

	D3D12_DESCRIPTOR_HEAP_DESC hdesc = { };
	hdesc.NumDescriptors = NUM_BACK_BUFFERS;
	hdesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	d3dDevice->CreateDescriptorHeap(&hdesc, IID_PPV_ARGS(&m_rtvHeap));

	hdesc.NumDescriptors = 1;
	hdesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	d3dDevice->CreateDescriptorHeap(&hdesc, IID_PPV_ARGS(&m_dsvHeap));

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
	for (int i = 0; i < NUM_BACK_BUFFERS; ++i) {
		m_backBufferViews[i] = rtvHeapHandle;
		dxgiSwapChain->GetBuffer(i, IID_PPV_ARGS(&m_backBuffers[i]));
		d3dDevice->CreateRenderTargetView(m_backBuffers[i], 0, rtvHeapHandle);

		rtvHeapHandle.ptr += rtvDescriptorSize;
	}

	// Depth buffer.
	D3D12_RESOURCE_DESC resDesc = { };
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resDesc.Width = width;
	resDesc.Height = height;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Format = m_depthBufferFormat;
	resDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	d3dDevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_COMMON, &CD3DX12_CLEAR_VALUE(m_depthBufferFormat, 1.0F, 0), IID_PPV_ARGS(&m_depthBuffer));

	d3dCommandAllocator->Reset();
	d3dCommandList->Reset(d3dCommandAllocator, 0);

	d3dCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_depthBuffer,D3D12_RESOURCE_STATE_COMMON,D3D12_RESOURCE_STATE_DEPTH_WRITE));

	d3dCommandList->Close();
	ID3D12CommandList* lists[] = { d3dCommandList };
	d3dCommandQueue->ExecuteCommandLists(1, lists);
	flushCommandQueue();

	m_depthBufferView = m_dsvHeap->GetCPUDescriptorHandleForHeapStart();
	d3dDevice->CreateDepthStencilView(m_depthBuffer, 0, m_dsvHeap->GetCPUDescriptorHandleForHeapStart());
}



//
// Shutdown.
//

void Graphics::_shutdown()
{
	m_renderScene.destroy();

	m_cbvHeap->Release();
	m_samplerHeap->Release();

	for (int i = 0; i < NUM_BACK_BUFFERS; ++i) {
		m_backBuffers[i]->Release();
	}
	m_depthBuffer->Release();

	m_rtvHeap->Release();
	m_dsvHeap->Release();

	dxgiSwapChain->Release();

	d3dCommandList->Release();
	d3dCommandAllocator->Release();
	d3dCommandQueue->Release();

	CloseHandle(m_fenceEvent);
	d3dFence->Release();

	d3dDevice->Release();
	dxgiFactory->Release();
}



void Graphics::renderFrame()
{
	this->beginFrame();


	// Render passes.
	m_renderScene.renderAll(d3dCommandList);


	this->endFrame();
	this->swapBuffers();

	nEntries = 0;
}

void Graphics::resizeBuffers(int width, int height)
{
	m_renderWidth = width;
	m_renderHeight = height;
}

UINT Graphics::allocateDescriptorTable(INT numDescriptors)
{
	UINT base = nEntries;
	nEntries += numDescriptors;
	return base;
}

void Graphics::setGlobalDescriptor(UINT index, D3D12_CPU_DESCRIPTOR_HANDLE descriptor)
{
	d3dDevice->CopyDescriptorsSimple(1,
		CD3DX12_CPU_DESCRIPTOR_HANDLE(m_cbvHeap->GetCPUDescriptorHandleForHeapStart(), index, cbvDescriptorSize),
		descriptor,
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}


void Graphics::flushCommandQueue()
{
	m_currentFenceValue++;

	d3dCommandQueue->Signal(d3dFence, m_currentFenceValue);

	if (d3dFence->GetCompletedValue() < m_currentFenceValue) {

		d3dFence->SetEventOnCompletion(m_currentFenceValue, m_fenceEvent);
		WaitForSingleObject(m_fenceEvent, INFINITE);
	}
}

void Graphics::beginFrame()
{
	this->executePendingTransfers();

	d3dCommandAllocator->Reset();
	d3dCommandList->Reset(d3dCommandAllocator, 0);

	d3dCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(this->getCurrentBackBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	D3D12_VIEWPORT viewport = { 0.0F, 0.0F, m_renderWidth, m_renderHeight, 0.0F, 1.0F };
	D3D12_RECT scissors = { 0, 0, m_renderWidth, m_renderHeight };
	d3dCommandList->RSSetViewports(1, &viewport);
	d3dCommandList->RSSetScissorRects(1, &scissors);

	// Set and clear back buffer and depth buffer.
	d3dCommandList->OMSetRenderTargets(1, &this->getCurrentBackBufferView(), false, &this->m_depthBufferView);

	float clearColor[4] = { 0.25F,0.15F,0.4F,1 };
	//d3dCommandList->ClearRenderTargetView(this->getCurrentBackBufferView(), clearColor, 0, 0);
	d3dCommandList->ClearDepthStencilView(this->m_depthBufferView, D3D12_CLEAR_FLAG_DEPTH, 1.0F, 0, 0, 0);

	ID3D12DescriptorHeap* dh[] = { m_cbvHeap, m_samplerHeap };
	d3dCommandList->SetDescriptorHeaps(2, dh);
}

void Graphics::endFrame()
{
	// Finish rendering and present back buffer.
	d3dCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(this->getCurrentBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	d3dCommandList->Close();

	ID3D12CommandList* lists[] = { d3dCommandList };
	d3dCommandQueue->ExecuteCommandLists(1, lists);
}

void Graphics::swapBuffers()
{
	// Wait for frame to complete.
	this->flushCommandQueue();

	dxgiSwapChain->Present(1, 0);

	m_currentBackBuffer = (m_currentBackBuffer + 1) % NUM_BACK_BUFFERS;
}

void Graphics::executePendingTransfers()
{
	if (m_resourceTransferUtility.performTransfers()) {
		this->flushCommandQueue();
		m_resourceTransferUtility.clear();
	}
}
