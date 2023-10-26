#include "Model.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"

Model::Model()
{
	VB = new VertexBuffer();
	IB = new IndexBuffer();
	numTris = 0;
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
	numTris = numTriangle;
}
