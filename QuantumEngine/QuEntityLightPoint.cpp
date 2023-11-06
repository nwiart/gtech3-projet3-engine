#include "QuEntityLightPoint.h"

#include <stdafx.h>
#include "Graphics.h"

QuEntityLightPoint::QuEntityLightPoint()
{
	m_color.fromFloat4(0.4f, 0.5F, 0.8F, 1.0F);

	m_intensity = 1.0F;
}

QuEntityLightPoint::~QuEntityLightPoint()
{
}

void QuEntityLightPoint::ExecuteProcedure()
{
	Graphics::getInstance().addPointLight(this);


}
