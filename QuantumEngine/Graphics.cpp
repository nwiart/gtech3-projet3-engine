#include "stdafx.h"
#include "Graphics.h"

#include "Timer.h"
#include "Window.h"

#include "QuEntityLightDirectional.h"



// Loading resources from the executable.
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
// Public API.
//

void Graphics::UpdateDirectionalLight(QuEntityLightDirectional* Entity)
{
	LightEntity = Entity;
}

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

void Graphics::createCompatiblePSO(Shader* shader)
{
	assert(shader->isReady());



	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,                 D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 3 * sizeof(float), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 6 * sizeof(float), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UINT,   0, 8 * sizeof(float), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc; ZeroMemory(&desc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	desc.pRootSignature = shader->getRootSignature();
	desc.VS = shader->getShaderBytecode<Shader::SHADER_VS>();
	desc.PS = shader->getShaderBytecode<Shader::SHADER_PS>();
	desc.InputLayout = { inputLayout, sizeof(inputLayout) / sizeof(inputLayout[0]) };
	desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	desc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	desc.SampleMask = UINT_MAX;
	desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.NumRenderTargets = 1;
	desc.RTVFormats[0] = m_backBufferFormat;
	desc.DSVFormat = m_depthBufferFormat;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	HRESULT r = d3dDevice->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&m_defaultPSO));
}



//
// Shutdown.
//

void Graphics::_shutdown()
{
	m_cbObjectData.destroy();
	m_cbFrameData.destroy();

	m_vb.destroy();
	m_ib.destroy();

	m_cbvHeap->Release();
	m_samplerHeap->Release();

	m_shader.destroy();
	m_defaultPSO->Release();

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



#include "Quantum/Generate/SphereGenerator.h"

void Graphics::initTestApp(UINT shaderResID)
{
	// Load our shader and our PSO.
	std::string source;
	loadResource(source, shaderResID);

	m_shader.setShaderSource<Shader::SHADER_VS>(source.c_str(), source.length());
	m_shader.setShaderSource<Shader::SHADER_PS>(source.c_str(), source.length());
	m_shader.compile();
	this->createCompatiblePSO(&m_shader);

	struct MyVertex { float pos[3]; float normal[3]; float uv[2]; UINT32 color; };

	MyVertex verts[] = {
		{ {-0.5F, -0.5F, -0.5F}, {-1, -1, -1}, {0, 0}, 0xFF000000 },
		{ {-0.5F, +0.5F, -0.5F}, {-1,  1, -1}, {0, 1}, 0xFF00FF00 },
		{ {+0.5F, +0.5F, -0.5F}, { 1,  1, -1}, {1, 1}, 0xFF00FFFF },
		{ {+0.5F, -0.5F, -0.5F}, { 1, -1, -1}, {1, 0}, 0xFF0000FF },
		{ {-0.5F, -0.5F, +0.5F}, {-1, -1,  1}, {0, 0}, 0xFFFF0000 },
		{ {-0.5F, +0.5F, +0.5F}, {-1,  1,  1}, {0, 1}, 0xFFFFFF00 },
		{ {+0.5F, +0.5F, +0.5F}, { 1,  1,  1}, {1, 1}, 0xFFFFFFFF },
		{ {+0.5F, -0.5F, +0.5F}, { 1, -1,  1}, {1, 0}, 0xFFFF00FF },
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

	//m_vb.setData(verts, sizeof(verts));
	//m_ib.setData(indices, sizeof(indices));

	m_cbFrameData.init();
	m_cbObjectData.init(65000);

	m_texture.loadFromDisk("awesome_sphere.dds");
}



static float angle1 = 0.0F;
static float angle2 = 0.0F;

void Graphics::update(const Timer& timer)
{
	TestConstantBuffer cb;
	{
		XMMATRIX view, projection;

		XMVECTOR pos = XMVectorSet(0.0F, 0.0F, -4.0F, 1.0F);
		XMVECTOR target = XMVectorSet(0.0F, 0.0F, 0.0F, 0.0F);
		XMVECTOR up = XMVectorSet(0.0F, 1.0F, 0.0F, 0.0F);
		XMVECTOR dir;
		dir = XMVectorSubtract(target, pos);
		dir = XMVector3Normalize(dir);

		view = XMMatrixLookAtLH(pos, target, up);
		projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(70.0F), m_renderWidth / (float) m_renderHeight, 0.05F, 1000.0F);

		// Combined view and projection matrices.
		XMStoreFloat4x4(&cb.viewProjection, view * projection);

		// Camera info.
		XMStoreFloat4(&cb.cameraPos, pos);
		XMStoreFloat4(&cb.cameraDir, dir);

		if (LightEntity == NULL)
		{
			cb.DirColors.x = 0;
			cb.DirColors.z = 0;
			cb.DirColors.y = 0;
			cb.DirColors.w = 0;
		}
		else
		{

			XMStoreFloat4(&cb.DirDirection, LightEntity->GetTransform().getForwardVector());
			cb.DirColors.x = LightEntity->getIntensity() * LightEntity->getColorR();
			cb.DirColors.y = LightEntity->getIntensity() * LightEntity->getColorG();
			cb.DirColors.z = LightEntity->getIntensity() * LightEntity->getColorB();
		}

		m_cbFrameData.update(0, cb);
	}

	angle1 += timer.getDeltaTime() * 4.0F;
	angle2 += timer.getDeltaTime();
}

void Graphics::renderFrame(const Timer& timer)
{
	update(timer);

	float yaw = (cursorX - m_renderWidth / 2) * -0.01F;
	float pitch = (cursorY - m_renderHeight / 2) * -0.01F;

	XMMATRIX objectTransform;


	objectTransform = XMMatrixRotationY(DirectX::XM_PI * -0.5F) * XMMatrixTranslation(-2.0F, sin(angle1) * 0.5F, 0.0F);

	objectTransform = XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0F);

	objectTransform = XMMatrixRotationY(angle2) * XMMatrixTranslation(2, 0, 0);

	this->beginFrame();

	d3dCommandList->SetPipelineState(m_defaultPSO);

	ID3D12DescriptorHeap* dh[] = { m_cbvHeap, m_samplerHeap };
	d3dCommandList->SetDescriptorHeaps(2, dh);
	d3dCommandList->SetGraphicsRootSignature(m_shader.getRootSignature());

	// Frame data.
	d3dDevice->CopyDescriptorsSimple(
		1,
		CD3DX12_CPU_DESCRIPTOR_HANDLE(m_cbvHeap->GetCPUDescriptorHandleForHeapStart(), 0, cbvDescriptorSize),
		m_cbFrameData.getDescriptor(),
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	m_shader.setConstantBuffer(2, 0);
	nEntries++;

	// Material data.
	d3dDevice->CopyDescriptorsSimple(
		1,
		CD3DX12_CPU_DESCRIPTOR_HANDLE(m_cbvHeap->GetCPUDescriptorHandleForHeapStart(), 2, cbvDescriptorSize),
		m_texture.getShaderResourceView(),
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	m_shader.setTexture2D(0, 1);
	nEntries++;
	nEntries++;

	// Render object.
	d3dCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	for (int i = 0; i < renderList.size(); i++) 
	{
		d3dCommandList->IASetVertexBuffers(0, 1, &renderList[i].model->GetVertexBuffer()->getVertexBufferView());
		d3dCommandList->IASetIndexBuffer(&renderList[i].model->GetIndexBuffer()->getIndexBufferView());
		d3dDevice->CopyDescriptorsSimple(
			1,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(m_cbvHeap->GetCPUDescriptorHandleForHeapStart(), nEntries, cbvDescriptorSize),
			m_cbObjectData.getDescriptor(i),
			D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		m_shader.setConstantBuffer(0, nEntries);

		nEntries++;
		d3dCommandList->DrawIndexedInstanced(renderList[i].model->GetNumberTriangle() * 3, 1, 0, 0, 0);
	}

	this->endFrame();
	this->swapBuffers();
	this->freeRenderModel();
}

void Graphics::resizeBuffers(int width, int height)
{
	m_renderWidth = width;
	m_renderHeight = height;
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

	float clearColor[4] = { 0.5F,0.5F,0.8F,1 };
	d3dCommandList->ClearRenderTargetView(this->getCurrentBackBufferView(), clearColor, 0, 0);
	d3dCommandList->ClearDepthStencilView(this->m_depthBufferView, D3D12_CLEAR_FLAG_DEPTH, 1.0F, 0, 0, 0);
}

void Graphics::endFrame()
{
	// Finish rendering and present back buffer.
	d3dCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(this->getCurrentBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
}

void Graphics::swapBuffers()
{
	d3dCommandList->Close();

	ID3D12CommandList* lists[] = { d3dCommandList };
	d3dCommandQueue->ExecuteCommandLists(1, lists);

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

void Graphics::addRenderModel(Model* model, DirectX::FXMMATRIX worldMatrix)
{
	RenderModel renderModel =
	{
		model,
		renderList.size(),
	};	
	ObjectConstantBuffer constbuff;
	XMStoreFloat4x4(&constbuff.world, worldMatrix);
	m_cbObjectData.update(renderList.size(), constbuff);
	renderList.push_back(renderModel);


}

void Graphics::freeRenderModel()
{
	nEntries = 0;
	renderList.clear();
}
