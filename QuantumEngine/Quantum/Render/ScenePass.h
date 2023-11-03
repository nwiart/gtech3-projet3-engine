#pragma once

#include "D3D12Texture.h"

#include "Quantum/Render/ShaderDefault.h"

#include "Quantum/Render/RenderModel.h"

class Model;



class ScenePass
{
	friend class RenderScene;

private:

	void init();
	void destroy();

	void renderAll(ID3D12GraphicsCommandList* cmdList, const std::vector<RenderModel>& models, UINT frameData, UINT matData, UINT objectBase);



private:

		/// Global shader & texture.
	ShaderDefault m_shader;
	D3D12Texture m_texture;
};
