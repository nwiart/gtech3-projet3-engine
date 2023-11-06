#include "MeteorGenerator.h"
#include "stdafx.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "Model.h"

#include "Quantum/Math/Math.h"

using namespace Quantum;
using namespace Quantum::Math;

#include <vector>

using namespace std;

#define TO_RADIANS (3.1415926535898 / 180.0)



void MeteorGenerator::generate(Model* outModel, float radius, int m_axisSubdivs, int m_heightSubdivs)
{
	struct Vertex { float pos[3]; float normal[3]; float uv[2]; UINT color; };


	size_t numVertices = (m_heightSubdivs - 1) * m_axisSubdivs + 2;
	size_t numFaces = m_axisSubdivs * m_heightSubdivs;

	vector<Vertex> vertices;
	vector<UINT> indices;

	vertices.reserve(numVertices);
	indices.reserve(numFaces * 2 * 3);

	// Subdivision vertices.
	for (float itheta = 0; itheta <= m_heightSubdivs; ++itheta)
	{
		float theta = (180.0F * itheta / m_heightSubdivs) * TO_RADIANS;
		float sin_theta = sin(theta);
		float cos_theta = cos(theta);

		for (int iphi = 0; iphi <= m_axisSubdivs; ++iphi)
		{
			float phi = (360.0F * iphi / m_axisSubdivs) * TO_RADIANS;

			float x = sin(phi) * sin_theta + randomFloat(-0.1F, 0.1F);
			float y = cos_theta + randomFloat(-0.1F, 0.1F);
			float z = cos(phi) * sin_theta + randomFloat(-0.1F, 0.1F);

			Vertex v;
			v.pos[0] = x * radius; v.pos[1] = y * radius; v.pos[2] = z * radius;
			v.normal[0] = x; v.normal[1] = y; v.normal[2] = z;
			v.uv[0] = 1.0F - (iphi / (float)m_axisSubdivs); v.uv[1] = itheta / (float)m_heightSubdivs;
			v.color = 0xFFFFFFFF;
			vertices.push_back(v);
		}
	}

	// Triangles.
	for (int sub = 0; sub < m_heightSubdivs; ++sub)
	{
		// First vertex of the current height loop.
		int baseIndex = sub * (m_axisSubdivs + 1);

		for (int face = 0; face < m_axisSubdivs; ++face)
		{
			int baseFaceIndex = baseIndex + face;

			UINT v0 = baseFaceIndex + 1;
			UINT v1 = baseFaceIndex;
			UINT v2 = baseFaceIndex + m_axisSubdivs + 1;
			indices.push_back(v0); indices.push_back(v1); indices.push_back(v2);

			v0 = baseFaceIndex + 1;
			v1 = baseFaceIndex + m_axisSubdivs + 1;
			v2 = baseFaceIndex + m_axisSubdivs + 2;
			indices.push_back(v0); indices.push_back(v1); indices.push_back(v2);
		}
	}


	outModel->GetVertexBuffer()->setData(vertices.data(), vertices.size() * sizeof(Vertex), sizeof(Vertex));
	outModel->GetIndexBuffer()->setData(indices.data(), indices.size() * sizeof(UINT));

	outModel->SetNumTriangle(m_axisSubdivs * m_heightSubdivs * 2);


	// AABB extends to radius.
	XMVECTOR aabbMin = XMVectorReplicate(-radius);
	XMVECTOR aabbMax = XMVectorReplicate(radius);
	outModel->setDimensions(aabbMin, aabbMax);
}
