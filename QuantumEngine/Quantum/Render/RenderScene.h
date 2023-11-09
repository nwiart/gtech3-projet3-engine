#pragma once

#include "D3D12ConstantBuffer.h"
#include "TestConstantBuffer.h"

#include "Quantum/Render/FrustumTest.h"

#include "Quantum/Render/ScenePass.h"
#include "Quantum/Render/SkyboxPass.h"
#include "Quantum/Render/ParticlesPass.h"
#include "Quantum/Render/SunPass.h"

#include "Quantum/Render/RenderModel.h"

#define QU_RENDER_MAX_MESHES 16384



class Model;

class QuEntityCamera;
class QuEntityRenderModel;
class QuEntityRenderSkybox;
class QuEntityParticleEmitter;
class QuEntityLightDirectional;
class QuEntityLightPoint;



class RenderScene
{
	friend class Graphics;

private:

	void init();
	void destroy();

	void renderAll(ID3D12GraphicsCommandList* cmdList);



	void addRenderModel(QuEntityRenderModel* model);
	void addParticleEmitter(QuEntityParticleEmitter* pe);

	void freeRenderModel();

	void setCamera(QuEntityCamera* camera);
	void setDirectionalLight(QuEntityLightDirectional* en);
	void addPointLight( QuEntityLightPoint* en);
	void setSkybox(QuEntityRenderSkybox* skybox);

private:

	void updateFrameCB();
	void updateObjectCB();



public:

	DirectX::XMMATRIX getViewMatrix() const;
	DirectX::XMMATRIX getProjectionMatrix() const;



private:

	struct MaterialConstantBuffer
	{
		float specularIntensity;
		char padding[256 - 4];
	};

	ScenePass m_passScene;
	SkyboxPass m_passSkybox;
	ParticlesPass m_passParticles;

	SunPass m_passSun;


		/// List of objects to render.
	std::vector<RenderModel> renderList;
	std::vector<ObjectConstantBuffer> renderWorldMatrices;
	std::vector<MaterialConstantBuffer> renderMaterials;


		/// Frame-invariant data (camera, view & projection, light sources...).
	D3D12ConstantBuffer<TestConstantBuffer> m_cbFrameData;

		/// Array of constant buffers for object transforms.
	D3D12ConstantBuffer<ObjectConstantBuffer> m_cbObjectData;

	D3D12ConstantBuffer<MaterialConstantBuffer> m_cbMaterialData;


		/// Cached data to send to the constant buffer.
	DirectX::XMVECTOR cameraPos;
	DirectX::XMVECTOR cameraTarget;
	DirectX::XMVECTOR cameraRight;
	DirectX::XMVECTOR cameraUp;
	float cameraFOV;
	float cameraAspect;

	QuEntityLightDirectional* m_directionalLight;
	std::vector<QuEntityLightPoint*> m_ListPointLight;


		/// Frustum.
	Frustum m_frustum;
};
