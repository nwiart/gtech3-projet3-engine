#include "stdafx.h"
#include "BoxGenerator.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "Model.h"

using namespace Quantum;

#include <vector>

using namespace std;


void Quantum::BoxGenerator::generate(Model* outModel)
{
    struct Vertex { float pos[3]; float normal[3]; float uv[2]; UINT color; };
        // create the vertices using the CUSTOMVERTEX struct
    vector<Vertex> vertices;
    vector<UINT> indices;


	size_t numVertices = 24;
	size_t numFaces = 6;

	vertices.reserve(numVertices);
	indices.reserve(numFaces * 2 * 3);



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



		Vertex v;
		// Top face
		vertices.push_back({ {-3.0f, 3.0f, -3.0f }, { 0,1,0 }, { 0, 1 }, 0xFFFFFFFF });
		vertices.push_back({ {-3.0f, 3.0f,  3.0f }, { 0,1,0 }, { 0, 0 }, 0xFFFFFFFF });    
		vertices.push_back({ { 3.0f, 3.0f,  3.0f }, { 0,1,0 }, { 1, 0 }, 0xFFFFFFFF });    
		vertices.push_back({ { 3.0f, 3.0f, -3.0f }, { 0,1,0 }, { 1, 1 }, 0xFFFFFFFF });    

		// Bottom face
		vertices.push_back({ {-3.0f, -3.0f, -3.0f }, { 0,-1,0 }, { 0, 1 }, 0xFFFFFFFF });
		vertices.push_back({ {-3.0f, -3.0f,  3.0f }, { 0,-1,0 }, { 0, 0 }, 0xFFFFFFFF });
		vertices.push_back({ { 3.0f, -3.0f,  3.0f }, { 0,-1,0 }, { 1, 0 }, 0xFFFFFFFF });
		vertices.push_back({ { 3.0f, -3.0f, -3.0f }, { 0,-1,0 }, { 1, 1 }, 0xFFFFFFFF });

		//back face
		vertices.push_back({ { -3.0f, -3.0f, 3.0f }, { 0,0,1 }, { 0, 1 }, 0xFFFFFFFF });
		vertices.push_back({ {  3.0f, -3.0f, 3.0f }, { 0,0,1 }, { 0, 0 }, 0xFFFFFFFF });
		vertices.push_back({ {  3.0f,  3.0f, 3.0f }, { 0,0,1 }, { 1, 0 }, 0xFFFFFFFF });
		vertices.push_back({ { -3.0f,  3.0f, 3.0f }, { 0,0,1 }, { 1, 1 }, 0xFFFFFFFF });

		//front face
		vertices.push_back({ { -3.0f, -3.0f, -3.0f }, { 0,0,-1 }, { 0, 1 }, 0xFFFFFFFF });
		vertices.push_back({ {  3.0f, -3.0f, -3.0f }, { 0,0,-1 }, { 0, 0 }, 0xFFFFFFFF });
		vertices.push_back({ {  3.0f,  3.0f, -3.0f }, { 0,0,-1 }, { 1, 0 }, 0xFFFFFFFF });
		vertices.push_back({ { -3.0f,  3.0f, -3.0f }, { 0,0,-1 }, { 1, 1 }, 0xFFFFFFFF });
		
		//Left face
		vertices.push_back({ {-3.0f, -3.0f, -3.0f }, { 0,-1,0 }, { 0, 1 }, 0xFFFFFFFF });
		vertices.push_back({ {-3.0f, -3.0f,  3.0f }, { 0,-1,0 }, { 0, 0 }, 0xFFFFFFFF });
		vertices.push_back({ { 3.0f, -3.0f,  3.0f }, { 0,-1,0 }, { 1, 0 }, 0xFFFFFFFF });
		vertices.push_back({ { 3.0f, -3.0f, -3.0f }, { 0,-1,0 }, { 1, 1 }, 0xFFFFFFFF });

		// right face
		vertices.push_back({ {-3.0f, -3.0f, -3.0f }, { 0,-1,0 }, { 0, 1 }, 0xFFFFFFFF });
		vertices.push_back({ {-3.0f, -3.0f,  3.0f }, { 0,-1,0 }, { 0, 0 }, 0xFFFFFFFF });
		vertices.push_back({ { 3.0f, -3.0f,  3.0f }, { 0,-1,0 }, { 1, 0 }, 0xFFFFFFFF });
		vertices.push_back({ { 3.0f, -3.0f, -3.0f }, { 0,-1,0 }, { 1, 1 }, 0xFFFFFFFF });

		
	

        outModel->GetVertexBuffer()->setData(vertices.data(), vertices.size() * sizeof(Vertex));
        outModel->GetIndexBuffer()->setData(indices.data(), indices.size() * sizeof(UINT));

        outModel->SetNumTriangle(numFaces * 2);
    }


