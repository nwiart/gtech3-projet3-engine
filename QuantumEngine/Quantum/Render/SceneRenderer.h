#pragma once

#include "D3D12ConstantBuffer.h"
#include "TestConstantBuffer.h"

#include "D3D12Texture.h"
#include "Shader.h"

class Model;

class QuEntityCamera;
class QuEntityLightDirectional;
class QuEntityLightPoint;



class SceneRenderer
{
	friend class Graphics;

private:

	void init();
	void destroy();

	void renderAll(ID3D12GraphicsCommandList* cmdList);

	void addRenderModel(Model* model, DirectX::FXMMATRIX worldMatrix);

	void freeRenderModel();

	void setCamera(class QuEntity* camera);
	void setDirectionalLight(QuEntityLightDirectional* en);
	void setPointLight(int index, QuEntityLightPoint* en);

	void updateFrameCB();
	void updateObjectCB();



private:

	struct RenderModel
	{
		Model* model;
		int cbID;
	};

		/// List of objects to render.
	std::vector<RenderModel> renderList;
	std::vector<ObjectConstantBuffer> renderWorldMatrices;

		/// Global shader & texture.
	Shader m_shader;
	D3D12Texture m_texture;

		/// Frame-invariant data (camera, view & projection, light sources...).
	D3D12ConstantBuffer<TestConstantBuffer> m_cbFrameData;

		/// Array of constant buffers for object transforms.
	D3D12ConstantBuffer<ObjectConstantBuffer> m_cbObjectData;



		/// Cached data to send to the constant buffer.
	DirectX::XMVECTOR cameraPos;
	DirectX::XMVECTOR cameraTarget;
	DirectX::XMVECTOR cameraUp;

	QuEntityLightDirectional* m_directionalLight;
};
