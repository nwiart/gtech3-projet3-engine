#include "QuEntityRenderModel.h"

#include "stdafx.h"
#include "Graphics.h"


QuEntityRenderModel::QuEntityRenderModel()
{
}

QuEntityRenderModel::~QuEntityRenderModel()
{
}

void QuEntityRenderModel::ExecuteProcedure()
{
	DirectX::XMVECTOR q = DirectX::XMQuaternionRotationRollPitchYaw(0.08F, 0.08F, 0.0F);
	this->applyRotation(q);

	Graphics::getInstance().addRenderModel(m_model, XMLoadFloat4x4(&this->GetWorldTransformMatrix()));
}
