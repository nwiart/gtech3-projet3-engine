#include "stdafx.h"
#include "SkyboxRenderer.h"



static struct { float pos[3]; } verts[] = {
	{ {-1.0F, -1.0F, -1.0F} },
	{ {-1.0F, +1.0F, -1.0F} },
	{ {+1.0F, +1.0F, -1.0F} },
	{ {+1.0F, -1.0F, -1.0F} },
	{ {-1.0F, -1.0F, +1.0F} },
	{ {-1.0F, +1.0F, +1.0F} },
	{ {+1.0F, +1.0F, +1.0F} },
	{ {+1.0F, -1.0F, +1.0F} },
};

static UINT indices[] = {
	// front face
	0, 1, 2,
	0, 2, 3,
	// back face
	4, 6, 5,
	4, 7, 6,
	// left face
	4, 5, 1,
	4, 1, 0,
	// right face
	3, 2, 6,
	3, 6, 7,
	// top face
	1, 5, 6,
	1, 6, 2,
	// bottom face
	4, 0, 3,
	4, 3, 7
};



void SkyboxRenderer::init()
{
	m_boxVB.setData(verts, sizeof(verts));
	m_boxIB.setData(indices, sizeof(indices));
}
