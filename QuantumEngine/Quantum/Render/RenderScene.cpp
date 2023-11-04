#include "stdafx.h"
#include "RenderScene.h"

#include "Graphics.h"

#include "TextureCube.h"

#include "QuEntityRenderModel.h"
#include "QuEntityLightDirectional.h"
#include "QuEntityRenderSkybox.h"
#include "QuEntityLightPoint.h"
#include "QuEntityCamera.h"



void RenderScene::init()
{
	// Default values.
	cameraPos    = XMVectorSet(0, 0, 0, 0);
	cameraTarget = XMVectorSet(0, 0, 1, 0);
	cameraUp     = XMVectorSet(0, 1, 0, 0);

	cameraFOV = 70.0F;
	cameraAspect = Graphics::getInstance().getRenderWidth() / (float) Graphics::getInstance().getRenderHeight();

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

	frustumBuild(m_frustum, cameraPos, cameraTarget, cameraRight, cameraUp, cameraFOV, cameraAspect);

	this->updateFrameCB();
	this->updateObjectCB();


	// Frame data.
	UINT cb_frameData_ID = g.allocateDescriptorTable(1);
	g.setGlobalDescriptor(cb_frameData_ID, m_cbFrameData.getDescriptor());

	// Material data.
	UINT cb_materialData_ID = g.allocateDescriptorTable(3);
	g.setGlobalDescriptor(cb_materialData_ID + 2, m_passSkybox.getTexture()->getShaderResourceView());

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



static void get_approximate_sphere(float& outSphereRadius, FXMVECTOR aabbMin, FXMVECTOR aabbMax);

void RenderScene::addRenderModel(QuEntityRenderModel* model)
{
	assert(renderList.size() < QU_RENDER_MAX_MESHES);

	// Frustum test.
	XMVECTOR worldPos = model->getWorldPosition();
	float radius = 0.0F;
	get_approximate_sphere(radius, XMLoadFloat3(&model->GetModel()->getDimensionsMin()), XMLoadFloat3(&model->GetModel()->getDimensionsMax()));

	if (!frustumSphereIntersect(m_frustum, worldPos, radius)) return;

	XMMATRIX worldMatrix = XMLoadFloat4x4(&model->GetWorldTransformMatrix());
	worldMatrix = XMMatrixTranspose(worldMatrix);

	RenderModel renderModel =
	{
		model->GetModel(),
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
	m_ListPointLight.clear();
}

void RenderScene::setCamera(QuEntityCamera* en)
{
	XMVECTOR fwd = en->getForwardVector();

	cameraPos = en->getWorldPosition();
	cameraTarget = XMVectorAdd(cameraPos, fwd);
	cameraRight = en->getRightVector();
	cameraUp = en->getUpVector();

	cameraFOV = en->getFOV();
}

void RenderScene::setDirectionalLight(QuEntityLightDirectional* en)
{
	m_directionalLight = en;
}

void RenderScene::addPointLight( QuEntityLightPoint* en)
{
	m_ListPointLight.push_back(en);
}

void RenderScene::setSkybox(QuEntityRenderSkybox* skybox)
{
	m_passSkybox.setTexture(skybox->getTexture()->getTexture());
}



void RenderScene::updateFrameCB()
{
	TestConstantBuffer cb;
	{
		XMMATRIX view, projection, viewProjection;

		// Combined view and projection matrices.
		view = XMMatrixLookAtLH(cameraPos, cameraTarget, cameraUp);
		projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(cameraFOV), cameraAspect, 0.05F, 1000.0F);

		viewProjection = XMMatrixMultiplyTranspose(view, projection);
		XMStoreFloat4x4(&cb.viewProjection, viewProjection);

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

		int i;
		for (i = 0; i < m_ListPointLight.size(); i++)
		{
			XMFLOAT4 colorPl;
			m_ListPointLight[i]->getColor().toFloat4(&colorPl.x);
			XMVECTOR colorIntensityPl = XMVectorMultiply(XMLoadFloat4(&colorPl), XMVectorReplicate(m_ListPointLight[i]->getIntensity()));
			XMStoreFloat4(&cb.PointLightColor[i], colorIntensityPl);

			XMStoreFloat4(&cb.PointLightPosition[i], m_ListPointLight[i]->getWorldPosition());
		}

		for (i; i < MAX_POINT_LIGHT; i++)
		{
			cb.PointLightColor[i] = XMFLOAT4(0, 0, 0, 1);
			cb.PointLightPosition[i] = XMFLOAT4(0, 0, 0, 1);
		}

		m_cbFrameData.update(0, cb);
	}
}

void RenderScene::updateObjectCB()
{
	m_cbObjectData.updateRange(0, renderWorldMatrices.size(), renderWorldMatrices.data());
}



static void get_approximate_sphere(float& outSphereRadius, FXMVECTOR aabbMin, FXMVECTOR aabbMax)
{
	float lenMin = XMVectorGetX(XMVector3Length(aabbMin));
	float lenMax = XMVectorGetX(XMVector3Length(aabbMax));

	outSphereRadius = max(lenMin, lenMax);
}
