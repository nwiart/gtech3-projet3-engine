#include "stdafx.h"
#include "RenderScene.h"

#include "Graphics.h"

#include "QuEntity.h"
#include "QuEntityLightDirectional.h"



void RenderScene::init()
{
	// Default values.
	cameraPos    = XMVectorSet(0, 0, 0, 0);
	cameraTarget = XMVectorSet(0, 0, 1, 0);
	cameraUp     = XMVectorSet(0, 1, 0, 0);

	m_cbFrameData.init();
	m_cbObjectData.init(QU_RENDER_MAX_MESHES);

	m_passScene.init();
	m_passSkybox.init();
}

void RenderScene::destroy()
{
	m_passSkybox.destroy();
	m_passScene.destroy();

	m_cbObjectData.destroy();
	m_cbFrameData.destroy();
}

void RenderScene::renderAll(ID3D12GraphicsCommandList* cmdList)
{
	Graphics& g = Graphics::getInstance();

	this->updateFrameCB();
	this->updateObjectCB();


	// Frame data.
	UINT cb_frameData_ID = g.allocateDescriptorTable(1);
	g.setGlobalDescriptor(cb_frameData_ID, m_cbFrameData.getDescriptor());

	// Material data.
	UINT cb_materialData_ID = g.allocateDescriptorTable(2);

	// Object world matrices.
	UINT cb_objectData_IDbase = g.allocateDescriptorTable(renderList.size());
	for (int i = 0; i < renderList.size(); i++)
	{
		g.setGlobalDescriptor(cb_objectData_IDbase + i, m_cbObjectData.getDescriptor(i));
	}


	m_passScene.renderAll(cmdList, renderList, cb_frameData_ID, cb_materialData_ID, cb_objectData_IDbase);

	m_passSkybox.render(cmdList, cb_frameData_ID);


	// Clear model list.
	this->freeRenderModel();
}



void RenderScene::addRenderModel(Model* model, DirectX::FXMMATRIX worldMatrix)
{
	assert(renderList.size() < QU_RENDER_MAX_MESHES);

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

void RenderScene::freeRenderModel()
{
	renderList.clear();
	renderWorldMatrices.clear();
}

void RenderScene::setCamera(QuEntity* camera)
{
	QuEntity* en = (QuEntity*)camera;

	XMVECTOR fwd = en->GetTransform().getForwardVector();

	cameraPos = en->getWorldPosition();
	cameraTarget = XMVectorAdd(cameraPos, fwd);
	cameraUp = en->getUpVector();
}

void RenderScene::setDirectionalLight(QuEntityLightDirectional* en)
{
	m_directionalLight = en;
}

void RenderScene::setPointLight(int index, QuEntityLightPoint* en)
{

}



void RenderScene::updateFrameCB()
{
	TestConstantBuffer cb;
	{
		XMMATRIX view, projection;

		// Combined view and projection matrices.
		float fov = 70.0F;
		float aspectRatio = Graphics::getInstance().getRenderWidth() / (float)Graphics::getInstance().getRenderHeight();

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

void RenderScene::updateObjectCB()
{
	m_cbObjectData.updateRange(0, renderWorldMatrices.size(), renderWorldMatrices.data());
}
