#include "Model.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"

using namespace DirectX;



Model::Model()
{
	VB = new VertexBuffer();
	IB = new IndexBuffer();
	m_numTriangles = 0;
}

Model::~Model()
{
	VB->destroy();
	delete VB;

	IB->destroy();
	delete IB;
}

void Model::SetNumTriangle(unsigned int numTriangle)
{
	m_numTriangles = numTriangle;
}



void Model::setDimensions(FXMVECTOR aabbMin, FXMVECTOR aabbMax)
{
	XMStoreFloat3(&m_dimensionsMin, aabbMin);
	XMStoreFloat3(&m_dimensionsMax, aabbMax);
}
