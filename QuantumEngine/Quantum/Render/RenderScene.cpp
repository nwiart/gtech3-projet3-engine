#include "stdafx.h"
#include "RenderScene.h"

#include "Graphics.h"

#include "Model.h"
#include "Texture2D.h"
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
	m_cbMaterialData.init(QU_RENDER_MAX_MESHES);

	m_passScene.init();
	m_passSkybox.init();
	m_passParticles.init();

	m_passSun.init();
}

void RenderScene::destroy()
{
	m_passParticles.destroy();
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

	// Material data & Object world matrices.
	UINT cb_materialData_ID = g.allocateDescriptorTable(3 * renderList.size());
	UINT cb_objectData_IDbase = g.allocateDescriptorTable(renderList.size());

	for (int i = 0; i < renderList.size(); i++)
	{
		g.setGlobalDescriptor(cb_objectData_IDbase + i, m_cbObjectData.getDescriptor(i));

		g.setGlobalDescriptor(cb_materialData_ID + i * 3, m_cbMaterialData.getDescriptor(i));

		// Set texture.
		D3D12Texture* tex = &m_passScene.m_texture;
		if (renderList[i].texture && renderList[i].texture->getTexture()) {
			tex = renderList[i].texture->getTexture();
		}
		g.setGlobalDescriptor(cb_materialData_ID + i * 3 + 1, tex->getShaderResourceView());

		// Set environment reflection map.
		if (m_passSkybox.getTexture()) {
			g.setGlobalDescriptor(cb_materialData_ID + i * 3 + 2, m_passSkybox.getTexture()->getShaderResourceView());
		}
	}


	m_passScene.renderAll(cmdList, renderList, cb_frameData_ID, cb_materialData_ID, cb_objectData_IDbase);

	m_passSkybox.render(cmdList, cb_frameData_ID);

	m_passParticles.render(cmdList);

	m_passSun.render(cmdList);


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
		model->GetModel()->getDefaultTexture(),
		renderList.size(),
	};

	ObjectConstantBuffer cb;
	XMStoreFloat4x4(&cb.world, worldMatrix);

	MaterialConstantBuffer matCb;
	matCb.specularIntensity = model->GetModel()->getSpecularIntensity();

	renderList.push_back(renderModel);
	renderWorldMatrices.push_back(cb);
	renderMaterials.push_back(matCb);
}

void RenderScene::addParticleEmitter(QuEntityParticleEmitter* pe)
{
	m_passParticles.addParticleEmitterData(pe);
}

void RenderScene::freeRenderModel()
{
	renderList.clear();
	renderWorldMatrices.clear();
	renderMaterials.clear();
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
		view = this->getViewMatrix();
		projection = this->getProjectionMatrix();

		viewProjection = XMMatrixMultiply(projection, view);
		XMStoreFloat4x4(&cb.viewProjection, viewProjection);

		// Camera info.
		XMStoreFloat4(&cb.cameraPos, cameraPos);

		// Directional light.
		if (m_directionalLight == NULL)
		{
			XMVECTOR zero = XMVectorZero();
			XMStoreFloat4(&cb.DirColors, zero);
			XMStoreFloat4(&cb.DirDirection, zero);
			XMStoreFloat4(&cb.AmbientColor, zero);
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
	m_cbMaterialData.updateRange(0, renderMaterials.size(), renderMaterials.data());
}

XMMATRIX RenderScene::getViewMatrix() const
{
	return XMMatrixTranspose(XMMatrixLookAtLH(cameraPos, cameraTarget, cameraUp));
}

XMMATRIX RenderScene::getProjectionMatrix() const
{
	return XMMatrixTranspose(XMMatrixPerspectiveFovLH(XMConvertToRadians(cameraFOV), cameraAspect, 0.05F, 5000.0F));
}



static void get_approximate_sphere(float& outSphereRadius, FXMVECTOR aabbMin, FXMVECTOR aabbMax)
{
	float lenMin = XMVectorGetX(XMVector3Length(aabbMin));
	float lenMax = XMVectorGetX(XMVector3Length(aabbMax));

	outSphereRadius = max(lenMin, lenMax);
}
