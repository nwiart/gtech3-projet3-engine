#include "stdafx.h"
#include "Game.h"

#include "../Game/resource.h"

#include "TestConstantBuffer.h"

#include "D3D12UploadHelper.h"

#include <iostream>
#include <cassert>
#include <string>
#include <ctime>



Game* Game::m_instance = 0;

static const char WINDOW_TITLE[] = "Direct3D 12 Test";



static void loadResource(std::string& out, UINT id)
{
	HINSTANCE hinstance = GetModuleHandle(0);

	HRSRC res = FindResource(hinstance, MAKEINTRESOURCE(id), RT_RCDATA);
	HGLOBAL resg = LoadResource(hinstance, res);
	SIZE_T size = SizeofResource(hinstance, res);

	out.assign(reinterpret_cast<const char*>(LockResource(resg)), size);

	FreeResource(resg);
}



//
// Update & Render.
//

void Game::update()
{
	TestConstantBuffer cb;
	{
		XMMATRIX view, projection;

		XMVECTOR pos = XMVectorSet(0.0F, 0.0F, -2.0F, 1.0F);
		XMVECTOR target = XMVectorSet(0.0F, 0.0F, 0.0F, 0.0F);
		XMVECTOR up = XMVectorSet(0.0F, 1.0F, 0.0F, 0.0F);
		view = XMMatrixLookAtLH(pos, target, up);

		projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(70.0F), m_windowWidth / (float) m_windowHeight, 0.05F, 1000.0F);

		cb.viewProjection = view * projection;

		m_cbFrameData.update(0, cb);
	}

	ObjectConstantBuffer ocb;
	{
		float yaw   = (cursorX - m_windowWidth / 2) * -0.003F;
		float pitch = (cursorY - m_windowHeight / 2) * -0.003F;

		XMMATRIX objectTransform;
		objectTransform = XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0F);

		ocb.world = objectTransform;

		m_cbObjectData.update(0, ocb);
	}
}

#define SET_CONSTANT_BUFFER(registerSlot, cbv) d3dDevice->CopyDescriptorsSimple(1, CD3DX12_CPU_DESCRIPTOR_HANDLE(m_cbvHeap->GetCPUDescriptorHandleForHeapStart(), registerSlot, cbvDescriptorSize), cbv, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

void Game::render()
{
	d3dCommandAllocator->Reset();
	d3dCommandList->Reset(d3dCommandAllocator, 0);

	d3dCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(this->getCurrentBackBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	D3D12_VIEWPORT viewport = { 0.0F, 0.0F, m_windowWidth, m_windowHeight, 0.0F, 1.0F };
	D3D12_RECT scissors = { 0, 0, m_windowWidth, m_windowHeight };
	d3dCommandList->RSSetViewports(1, &viewport);
	d3dCommandList->RSSetScissorRects(1, &scissors);

	// Set and clear back buffer and depth buffer.
	d3dCommandList->OMSetRenderTargets(1, &this->getCurrentBackBufferView(), false, &this->depthBufferView);

	float clearColor[4] = { 0.5F,0.5F,0.8F,1 };
	d3dCommandList->ClearRenderTargetView(this->getCurrentBackBufferView(), clearColor, 0, 0);
	d3dCommandList->ClearDepthStencilView(this->depthBufferView, D3D12_CLEAR_FLAG_DEPTH, 1.0F, 0, 0, 0);

	{
		d3dCommandList->SetPipelineState(m_defaultPSO);

		SET_CONSTANT_BUFFER(0, m_cbFrameData.getDescriptor());
		SET_CONSTANT_BUFFER(1, m_cbObjectData.getDescriptor());

		ID3D12DescriptorHeap* dh[] = { m_cbvHeap.Get(), m_samplerHeap.Get() };
		d3dCommandList->SetDescriptorHeaps(2, dh);
		d3dCommandList->SetGraphicsRootSignature(m_shader.getRootSignature());
		d3dCommandList->SetGraphicsRootDescriptorTable(0, m_cbvHeap->GetGPUDescriptorHandleForHeapStart());

		// Render object.
		d3dCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		d3dCommandList->IASetVertexBuffers(0, 1, &m_vb.getVertexBufferView());
		d3dCommandList->IASetIndexBuffer(&m_ib.getIndexBufferView());

		d3dCommandList->DrawIndexedInstanced(6 * 6, 1, 0, 0, 0);
	}

	// Finish rendering and present back buffer.
	d3dCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(this->getCurrentBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	d3dCommandList->Close();

	ID3D12CommandList* lists[] = { d3dCommandList };
	d3dCommandQueue->ExecuteCommandLists(1, lists);

	flushCommandQueue();

	dxgiSwapChain->Present(1, 0);

	currentBackBuffer = (currentBackBuffer + 1) % m_numBackBuffers;
}

void Game::flushCommandQueue()
{
	m_currentFenceValue++;

	d3dCommandQueue->Signal(d3dFence, m_currentFenceValue);

	if (d3dFence->GetCompletedValue() < m_currentFenceValue) {

		d3dFence->SetEventOnCompletion(m_currentFenceValue, m_fenceEvent);
		WaitForSingleObject(m_fenceEvent, INFINITE);
	}
}



//
// Initialization.
//

Game::Game()
{
	m_instance = this;
}

int Game::init(int windowWidth, int windowHeight)
{
	m_window.initialize(windowWidth, windowHeight, WINDOW_TITLE, false);
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;

	HWND hwnd = m_window.getHwnd();

#ifdef _DEBUG
	D3D12GetDebugInterface(IID_PPV_ARGS(&d3dDebug));
	d3dDebug->EnableDebugLayer();
#endif

	CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));

	// Device.
	HRESULT result = D3D12CreateDevice(0, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&d3dDevice));
	if (FAILED(result)) {
		return 1;
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
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = { };
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	d3dDevice->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&d3dCommandQueue));

	d3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&d3dCommandAllocator));

	d3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, d3dCommandAllocator, 0, IID_PPV_ARGS(&d3dCommandList));
	d3dCommandList->Close();

	// Swap chain.
	m_numBackBuffers = 2;
	m_backBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	DXGI_SWAP_CHAIN_DESC sd = { 0 };
	sd.BufferDesc.Format = m_backBufferFormat;
	sd.BufferDesc.Width = windowWidth;
	sd.BufferDesc.Height = windowHeight;
	sd.BufferCount = m_numBackBuffers;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.SampleDesc.Quality = 0;
	sd.SampleDesc.Count = 1;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.OutputWindow = hwnd;
	sd.Windowed = true;

	dxgiFactory->CreateSwapChain(d3dCommandQueue, &sd, &dxgiSwapChain);

	// Buffers & depth buffer.
	currentBackBuffer = 0;

	D3D12_DESCRIPTOR_HEAP_DESC hdesc = { };
	hdesc.NumDescriptors = m_numBackBuffers;
	hdesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	d3dDevice->CreateDescriptorHeap(&hdesc, IID_PPV_ARGS(&rtvHeap));

	hdesc.NumDescriptors = 1;
	hdesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	d3dDevice->CreateDescriptorHeap(&hdesc, IID_PPV_ARGS(&dsvHeap));

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	for (int i = 0; i < m_numBackBuffers; ++i) {
		backBufferViews[i] = rtvHeapHandle;
		dxgiSwapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));
		d3dDevice->CreateRenderTargetView(backBuffers[i], 0, rtvHeapHandle);

		rtvHeapHandle.ptr += rtvDescriptorSize;
	}

	this->createDepthBuffer(windowWidth, windowHeight);

	m_resourceTransferUtility.init(d3dCommandQueue, d3dCommandAllocator, d3dCommandList);


	D3D12_DESCRIPTOR_HEAP_DESC spDesc;
	spDesc.NumDescriptors = 16;
	spDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	spDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	spDesc.NodeMask = 0;
	d3dDevice->CreateDescriptorHeap(&spDesc, IID_PPV_ARGS(&m_cbvHeap));

	spDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
	spDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	d3dDevice->CreateDescriptorHeap(&spDesc, IID_PPV_ARGS(&m_samplerHeap));


	// Load our shader and our PSO.
	std::string vs_source, ps_source;
	loadResource(vs_source, ID_SHADER_TEST_VS);
	loadResource(ps_source, ID_SHADER_TEST_PS);

	m_shader.setShaderSource<Shader::SHADER_VS>(vs_source.c_str(), vs_source.length());
	m_shader.setShaderSource<Shader::SHADER_PS>(ps_source.c_str(), ps_source.length());
	m_shader.compile();
	this->createCompatiblePSO(&m_shader);

	struct MyVertex { float pos[3]; UINT32 color; };

	MyVertex verts[] = {
		{ {-0.5F, -0.5F, -0.5F}, 0xFF000000 },
		{ {-0.5F, +0.5F, -0.5F}, 0xFF00FF00 },
		{ {+0.5F, +0.5F, -0.5F}, 0xFF00FFFF },
		{ {+0.5F, -0.5F, -0.5F}, 0xFF0000FF },
		{ {-0.5F, -0.5F, +0.5F}, 0xFFFF0000 },
		{ {-0.5F, +0.5F, +0.5F}, 0xFFFFFF00 },
		{ {+0.5F, +0.5F, +0.5F}, 0xFFFFFFFF },
		{ {+0.5F, -0.5F, +0.5F}, 0xFFFF00FF },
	};
	UINT indices[] = {
		// front face
		0, 1, 2,
		0, 2, 3,
		// back face
		4, 6, 5,
		4, 7, 6,
		// left face
		4, 5, 1,
		4, 1, 0,
		// right face
		3, 2, 6,
		3, 6, 7,
		// top face
		1, 5, 6,
		1, 6, 2,
		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	m_vb.setData(verts, sizeof(verts));
	m_ib.setData(indices, sizeof(indices));

	m_cbFrameData.init();
	m_cbObjectData.init();

	return 0;
}

void Game::createDepthBuffer(int width, int height)
{
	D3D12_RESOURCE_DESC resDesc = { };
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resDesc.Width = width;
	resDesc.Height = height;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Format = m_depthBufferFormat;
	resDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	d3dDevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &CD3DX12_CLEAR_VALUE(m_depthBufferFormat, 1.0F, 0), IID_PPV_ARGS(&depthBuffer));

	depthBufferView = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	d3dDevice->CreateDepthStencilView(depthBuffer, 0, dsvHeap->GetCPUDescriptorHandleForHeapStart());
}

void Game::createCompatiblePSO(Shader* shader)
{
	assert(shader->isReady());

	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc; ZeroMemory(&desc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	desc.pRootSignature = shader->getRootSignature();
	desc.VS = shader->getShaderBytecode<Shader::SHADER_VS>();
	desc.PS = shader->getShaderBytecode<Shader::SHADER_PS>();
	desc.InputLayout = { inputLayout, sizeof(inputLayout) / sizeof(inputLayout[0]) };
	desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	desc.SampleMask = UINT_MAX;
	desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.NumRenderTargets = 1;
	desc.RTVFormats[0] = m_backBufferFormat;
	desc.DSVFormat = m_depthBufferFormat;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	d3dDevice->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&m_defaultPSO));
}



//
// Shutdown.
//

void Game::shutdown()
{
	m_cbObjectData.destroy();
	m_cbFrameData.destroy();

	m_vb.destroy();
	m_ib.destroy();

	m_shader.destroy();
	m_defaultPSO->Release();

	for (int i = 0; i < m_numBackBuffers; ++i) {
		backBuffers[i]->Release();
	}
	depthBuffer->Release();

	rtvHeap->Release();
	dsvHeap->Release();

	dxgiSwapChain->Release();

	d3dCommandList->Release();
	d3dCommandAllocator->Release();
	d3dCommandQueue->Release();

	CloseHandle(m_fenceEvent);
	d3dFence->Release();

	d3dDevice->Release();
	dxgiFactory->Release();
}

void Game::mainLoop()
{
	time_t currentTime = time(0);
	time_t lastTime = currentTime;

	int frames = 0;

	while (!m_window.wantsToClose())
	{
		m_window.pollEvents();

		if (m_resourceTransferUtility.performTransfers()) {
			this->flushCommandQueue();
			m_resourceTransferUtility.clear();
		}

		update();

		render();

		// Display FPS each second.
		frames++;
		currentTime = time(0);
		if (currentTime != lastTime) {

			std::string title = WINDOW_TITLE;
			title += " - ";
			title += std::to_string(frames);
			title += " FPS";
			
			m_window.setTitle(title.c_str());

			lastTime = currentTime;
			frames = 0;
		}
	}
}
