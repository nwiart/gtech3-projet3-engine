#include "stdafx.h"
#include "BoxGenerator.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "Model.h"

using namespace Quantum;

#include <vector>

using namespace std;


void Quantum::BoxGenerator::generate(Model* outModel, DirectX::FXMVECTOR halfSize)
{
    struct Vertex { float pos[3]; float normal[3]; float uv[2]; UINT color; };

    vector<Vertex> vertices;
    vector<UINT> indices;


	size_t numVertices = 24;
	size_t numFaces = 6;

	vertices.reserve(numVertices);
	indices.reserve(numFaces * 2 * 3);


	XMFLOAT3 size; XMStoreFloat3(&size, halfSize);



	for (int face = 0; face < numFaces; ++face)
	{
		int baseFaceIndex = face * 4;

		UINT v0 = baseFaceIndex;
		UINT v1 = baseFaceIndex + 1;
		UINT v2 = baseFaceIndex + 2;
		indices.push_back(v0); indices.push_back(v1); indices.push_back(v2);

		v0 = baseFaceIndex + 2;
		v1 = baseFaceIndex + 3;
		v2 = baseFaceIndex;
		indices.push_back(v0); indices.push_back(v1); indices.push_back(v2);
	}


	// Top face
	vertices.push_back({ { -size.x,  size.y, -size.z }, { 0,1,0 }, { 0, 1 }, 0xFFFFFFFF });
	vertices.push_back({ { -size.x,  size.y,  size.z }, { 0,1,0 }, { 0, 0 }, 0xFFFFFFFF });
	vertices.push_back({ {  size.x,  size.y,  size.z }, { 0,1,0 }, { 1, 0 }, 0xFFFFFFFF });
	vertices.push_back({ {  size.x,  size.y, -size.z }, { 0,1,0 }, { 1, 1 }, 0xFFFFFFFF });

	// Bottom face
	vertices.push_back({ {  size.x, -size.y, -size.z }, { 0,-1,0 }, { 0, 1 }, 0xFFFFFFFF });
	vertices.push_back({ {  size.x, -size.y,  size.z }, { 0,-1,0 }, { 0, 0 }, 0xFFFFFFFF });
	vertices.push_back({ { -size.x, -size.y,  size.z }, { 0,-1,0 }, { 1, 0 }, 0xFFFFFFFF });
	vertices.push_back({ { -size.x, -size.y, -size.z }, { 0,-1,0 }, { 1, 1 }, 0xFFFFFFFF });

	// Front face
	vertices.push_back({ {  size.x, -size.y,  size.z }, { 0,0,1 }, { 0, 1 }, 0xFFFFFFFF });
	vertices.push_back({ {  size.x,  size.y,  size.z }, { 0,0,1 }, { 0, 0 }, 0xFFFFFFFF });
	vertices.push_back({ { -size.x,  size.y,  size.z }, { 0,0,1 }, { 1, 0 }, 0xFFFFFFFF });
	vertices.push_back({ { -size.x, -size.y,  size.z }, { 0,0,1 }, { 1, 1 }, 0xFFFFFFFF });

	// Back face
	vertices.push_back({ { -size.x, -size.y, -size.z }, { 0,0,-1 }, { 0, 1 }, 0xFFFFFFFF });
	vertices.push_back({ { -size.x,  size.y, -size.z }, { 0,0,-1 }, { 0, 0 }, 0xFFFFFFFF });
	vertices.push_back({ {  size.x,  size.y, -size.z }, { 0,0,-1 }, { 1, 0 }, 0xFFFFFFFF });
	vertices.push_back({ {  size.x, -size.y, -size.z }, { 0,0,-1 }, { 1, 1 }, 0xFFFFFFFF });
		
	// Right face
	vertices.push_back({ { size.x, -size.y, -size.z }, { 1,0,0 }, { 0, 1 }, 0xFFFFFFFF });
	vertices.push_back({ { size.x,  size.y, -size.z }, { 1,0,0 }, { 0, 0 }, 0xFFFFFFFF });
	vertices.push_back({ { size.x,  size.y,  size.z }, { 1,0,0 }, { 1, 0 }, 0xFFFFFFFF });
	vertices.push_back({ { size.x, -size.y,  size.z }, { 1,0,0 }, { 1, 1 }, 0xFFFFFFFF });

	// Left face
	vertices.push_back({ {-size.x, -size.y,  size.z }, { -1,0,0 }, { 0, 1 }, 0xFFFFFFFF });
	vertices.push_back({ {-size.x,  size.y,  size.z }, { -1,0,0 }, { 0, 0 }, 0xFFFFFFFF });
	vertices.push_back({ {-size.x,  size.y, -size.z }, { -1,0,0 }, { 1, 0 }, 0xFFFFFFFF });
	vertices.push_back({ {-size.x, -size.y, -size.z }, { -1,0,0 }, { 1, 1 }, 0xFFFFFFFF });


    outModel->GetVertexBuffer()->setData(vertices.data(), vertices.size() * sizeof(Vertex), sizeof(Vertex));
    outModel->GetIndexBuffer()->setData(indices.data(), indices.size() * sizeof(UINT));

    outModel->SetNumTriangle(numFaces * 2);


	// AABB centered on the origin.
	XMVECTOR maxDist = XMVector3Length(halfSize);
	XMVECTOR aabbMin = -maxDist;
	XMVECTOR aabbMax =  maxDist;
	outModel->setDimensions(aabbMin, aabbMax);
}
