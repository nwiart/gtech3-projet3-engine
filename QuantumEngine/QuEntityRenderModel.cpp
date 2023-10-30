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
	Graphics::getInstance().addRenderModel(m_model, XMLoadFloat4x4(&this->GetWorldTransformMatrix()));
}
