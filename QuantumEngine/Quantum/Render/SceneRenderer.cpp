#include "stdafx.h"
#include "SceneRenderer.h"

#include "Graphics.h"

#include "Model.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "QuEntity.h"
#include "QuEntityLightDirectional.h"

#include "resource.h"



void loadResource(std::string& out, UINT id);



void SceneRenderer::init()
{
	// Default values.
	cameraPos    = XMVectorSet(0, 0, -10, 0);
	cameraTarget = XMVectorSet(0, 0, 1, 0);
	cameraUp     = XMVectorSet(0, 1, 0, 0);

	// Load our shader and our PSO.
	std::string source;
	loadResource(source, ID_SHADER_TEST);

	m_shader.compileShaderSource<Shader::SHADER_VS>(source.c_str(), source.length());
	m_shader.compileShaderSource<Shader::SHADER_PS>(source.c_str(), source.length());
	m_shader.compile();
	m_shader.createPSOs();
	assert(m_shader.isReady());


	m_cbFrameData.init();
	m_cbObjectData.init(65000);

	m_texture.loadFromDisk("awesome_sphere.dds", D3D12_SRV_DIMENSION_TEXTURE2D);
}

void SceneRenderer::destroy()
{
	m_cbFrameData.destroy();
	m_cbObjectData.destroy();

	m_shader.destroy();

	m_texture.destroy();
}

void SceneRenderer::renderAll(ID3D12GraphicsCommandList* cmdList)
{
	Graphics& g = Graphics::getInstance();

	this->updateFrameCB();
	this->updateObjectCB();

	// Bind shader.
	cmdList->SetPipelineState(m_shader.getPipelineStateObject());
	cmdList->SetGraphicsRootSignature(m_shader.getRootSignature());

	// Frame data.
	UINT index = g.allocateDescriptorTable(1);
	g.setGlobalDescriptor(index, m_cbFrameData.getDescriptor());
	m_shader.setConstantBuffer(2, index);

	// Material data.
	index = g.allocateDescriptorTable(2);
	g.setGlobalDescriptor(index + 1, m_texture.getShaderResourceView());
	m_shader.setTexture2D(0, index);

	// Render object.
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	for (int i = 0; i < renderList.size(); i++)
	{
		cmdList->IASetVertexBuffers(0, 1, &renderList[i].model->GetVertexBuffer()->getVertexBufferView());
		cmdList->IASetIndexBuffer(&renderList[i].model->GetIndexBuffer()->getIndexBufferView());

		UINT objectCBVIndex = g.allocateDescriptorTable(1);
		g.setGlobalDescriptor(objectCBVIndex, m_cbObjectData.getDescriptor(i));
		m_shader.setConstantBuffer(0, objectCBVIndex);

		cmdList->DrawIndexedInstanced(renderList[i].model->GetNumberTriangle() * 3, 1, 0, 0, 0);
	}


	// Clear model list.
	this->freeRenderModel();
}

void SceneRenderer::addRenderModel(Model* model, DirectX::FXMMATRIX worldMatrix)
{
	RenderModel renderModel =
	{
		model,
		renderList.size(),
	};

	ObjectConstantBuffer cb;
	XMStoreFloat4x4(&cb.world, worldMatrix);

	renderList.push_back(renderModel);
	renderWorldMatrices.push_back(cb);
}

void SceneRenderer::freeRenderModel()
{
	renderList.clear();
	renderWorldMatrices.clear();
}

void SceneRenderer::setCamera(QuEntity* camera)
{
	QuEntity* en = (QuEntity*) camera;

	XMVECTOR fwd = en->GetTransform().getForwardVector();

	cameraPos    = en->getWorldPosition();
	cameraTarget = XMVectorAdd(cameraPos, fwd);
	cameraUp     = en->getUpVector();
}

void SceneRenderer::setDirectionalLight(QuEntityLightDirectional* en)
{
	m_directionalLight = en;
}

void SceneRenderer::setPointLight(int index, QuEntityLightPoint* en)
{

}



void SceneRenderer::updateFrameCB()
{
	TestConstantBuffer cb;
	{
		XMMATRIX view, projection;

		// Combined view and projection matrices.
		float fov = 70.0F;
		float aspectRatio = Graphics::getInstance().getRenderWidth() / (float) Graphics::getInstance().getRenderHeight();

		view = XMMatrixLookAtLH(cameraPos, cameraTarget, cameraUp);
		projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(fov), aspectRatio, 0.05F, 1000.0F);

		XMStoreFloat4x4(&cb.viewProjection, view * projection);

		// Camera info.
		XMStoreFloat4(&cb.cameraPos, cameraPos);

		// Directional light.
		if (m_directionalLight == NULL)
		{
			cb.DirColors = XMFLOAT4(0, 0, 0, 0);
			cb.AmbientColor = XMFLOAT4(0, 0, 0, 0);
		}
		else
		{
			XMFLOAT4 color;
			XMVECTOR colorIntensity;

			// Direction.
			XMStoreFloat4(&cb.DirDirection, m_directionalLight->GetTransform().getForwardVector());

			// Color pre-multiplied by intensity.
			m_directionalLight->getColor().toFloat4(&color.x);
			colorIntensity = XMVectorMultiply(XMLoadFloat4(&color), XMVectorReplicate(m_directionalLight->getIntensity()));
			XMStoreFloat4(&cb.DirColors, colorIntensity);

			// Ambient color.
			m_directionalLight->getAmbientColor().toFloat4(&cb.AmbientColor.x);
		}

		m_cbFrameData.update(0, cb);
	}
}

void SceneRenderer::updateObjectCB()
{
	m_cbObjectData.updateRange(0, renderWorldMatrices.size(), renderWorldMatrices.data());
}
