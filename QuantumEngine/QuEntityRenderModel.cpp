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
	Graphics::getInstance().addRenderModel(this);
}
