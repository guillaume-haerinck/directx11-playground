#include "pch.h"
#include "MeshPrimitiveFactory.h"

#include "components/graphics/Mesh.h"

MeshPrimitiveFactory::MeshPrimitiveFactory(Context& context) : m_ctx(context) {
	m_ied.at(0) = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	m_ied.at(1) = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	m_ied.at(2) = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
}

MeshPrimitiveFactory::~MeshPrimitiveFactory()
{
}

comp::Mesh MeshPrimitiveFactory::CreateUVSphere(float radius, float sectorCount, float stackCount) {
	float x, y, z, xy;								// vertex position
	float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
	float s, t;                                     // vertex texCoord

	float sectorStep = 2 * XM_PI / sectorCount;
	float stackStep = XM_PI / stackCount;
	float sectorAngle, stackAngle;

	// Vertices
	std::vector<Vertex> vertices;
	for (int i = 0; i <= stackCount; ++i) {
		stackAngle = XM_PIDIV2 - i * stackStep;     // starting from pi/2 to -pi/2
		xy = radius * cosf(stackAngle);             // r * cos(u)
		z = radius * sinf(stackAngle);              // r * sin(u)

		// add (sectorCount+1) vertices per stack
		for (int j = 0; j <= sectorCount; ++j) {
			sectorAngle = j * sectorStep;           // starting from 0 to 2pi

			// vertex position
			x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
			y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)

			// normalized vertex normal (nx, ny, nz)
			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;

			// vertex tex coord (s, t) range between [0, 1]
			s = (float)j / sectorCount;
			t = (float)i / stackCount;

			vertices.push_back({ XMFLOAT3(x, y, z), XMFLOAT3(nx, ny, nz), XMFLOAT2(s, t) });
		}
	}

	// Indices
	int k1, k2;
	std::vector<WORD> indices;
	for (int i = 0; i < stackCount; ++i) {
		k1 = i * (sectorCount + 1);     // beginning of current stack
		k2 = k1 + sectorCount + 1;      // beginning of next stack

		for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
			// 2 triangles per sector excluding first and last stacks
			if (i != 0) {
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			if (i != (stackCount - 1)) {
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}

	// Create UV sphere
	comp::VertexBuffer vertexBuffer = m_ctx.rcommand->CreateVertexBuffer(vertices.data(), vertices.size(), sizeof(Vertex));
	comp::IndexBuffer indexBuffer = m_ctx.rcommand->CreateIndexBuffer(indices.data(), indices.size());

	// Store data
	comp::Mesh mesh = {};
	mesh.ib = indexBuffer;
	mesh.vb = vertexBuffer;
	return mesh;
}

comp::Mesh MeshPrimitiveFactory::CreateIcoSphere(float radius, unsigned int subdivisionCount) {
	comp::Mesh mesh = {};
	return mesh;
}

comp::Mesh MeshPrimitiveFactory::CreateBox(float width, float height) {
	//    v6----- v5
	//   /|      /|
	//  v1------v0|
	//  | |     | |
	//  | |v7---|-|v4
	//  |/      |/
	//  v2------v3
	Vertex vertices[] = {
		// Front v0,v1,v2,v3
		{ XMFLOAT3(1, 1, 1),   XMFLOAT3(0, 0, 1), XMFLOAT2(1, 0) },
		{ XMFLOAT3(-1, 1, 1),  XMFLOAT3(0, 0, 1), XMFLOAT2(0, 0) },
		{ XMFLOAT3(-1, -1, 1), XMFLOAT3(0, 0, 1), XMFLOAT2(0, 1) },
		{ XMFLOAT3(1, -1, 1),  XMFLOAT3(0, 0, 1), XMFLOAT2(1, 1) },

		// Right v0,v3,v4,v5
		{ XMFLOAT3(1, 1, 1),    XMFLOAT3(1, 0, 0), XMFLOAT2(0, 0) },
		{ XMFLOAT3(1, -1, 1),   XMFLOAT3(1, 0, 0), XMFLOAT2(0, 1) },
		{ XMFLOAT3(1, -1, -1),  XMFLOAT3(1, 0, 0), XMFLOAT2(1, 1) },
		{ XMFLOAT3(1, 1, -1),   XMFLOAT3(1, 0, 0), XMFLOAT2(1, 0) },

		// Top v0,v5,v6,v1								 
		{ XMFLOAT3(1, 1, 1),    XMFLOAT3(0, 1, 0), XMFLOAT2(1, 1) },
		{ XMFLOAT3(1, 1, -1),   XMFLOAT3(0, 1, 0), XMFLOAT2(1, 0) },
		{ XMFLOAT3(-1, 1, -1),  XMFLOAT3(0, 1, 0), XMFLOAT2(0, 0) },
		{ XMFLOAT3(-1, 1, 1),   XMFLOAT3(0, 1, 0), XMFLOAT2(0, 1) },

		// Left v1,v6,v7,v2						 
		{ XMFLOAT3(-1, 1, 1),   XMFLOAT3(-1, 0, 0), XMFLOAT2(1, 0) },
		{ XMFLOAT3(-1, 1, -1),  XMFLOAT3(-1, 0, 0), XMFLOAT2(0, 0) },
		{ XMFLOAT3(-1, -1, -1), XMFLOAT3(-1, 0, 0), XMFLOAT2(0, 1) },
		{ XMFLOAT3(-1, -1, 1),  XMFLOAT3(-1, 0, 0), XMFLOAT2(1, 1) },

		// Bottom v7,v4,v3,v2						 
		{ XMFLOAT3(-1, -1, -1), XMFLOAT3(0,-1, 0), XMFLOAT2(0, 1) },
		{ XMFLOAT3(1, -1, -1),  XMFLOAT3(0,-1, 0), XMFLOAT2(1, 1) },
		{ XMFLOAT3(1, -1, 1),   XMFLOAT3(0,-1, 0), XMFLOAT2(1, 0) },
		{ XMFLOAT3(-1, -1, 1),  XMFLOAT3(0,-1, 0), XMFLOAT2(0, 0) },

		// Back v4,v7,v6,v5					
		{ XMFLOAT3(1, -1, -1),  XMFLOAT3(0, 0,-1), XMFLOAT2(0, 1) },
		{ XMFLOAT3(-1, -1, -1), XMFLOAT3(0, 0,-1), XMFLOAT2(1, 1) },
		{ XMFLOAT3(-1, 1, -1),  XMFLOAT3(0, 0,-1), XMFLOAT2(1, 0) },
		{ XMFLOAT3(1, 1, -1),   XMFLOAT3(0, 0,-1), XMFLOAT2(0, 0) },
	};

	WORD indices[] = {
		0, 2, 1,   2, 0, 3,       // front
		4, 6, 5,   6, 4, 7,       // right
		8, 10,9,   10,8, 11,      // top
		12,14,13,  14,12,15,      // left
		16,18,17,  18,16,19,      // bottom
		20,22,21,  22,20,23		  // back
	};

	// Create Box
	comp::VertexBuffer vertexBuffer = m_ctx.rcommand->CreateVertexBuffer(vertices, ARRAYSIZE(vertices), sizeof(Vertex));
	comp::IndexBuffer indexBuffer = m_ctx.rcommand->CreateIndexBuffer(indices, ARRAYSIZE(indices));

	// Store data
	comp::Mesh mesh = {};
	mesh.ib = indexBuffer;
	mesh.vb = vertexBuffer;
	return mesh;
}

comp::Mesh MeshPrimitiveFactory::CreateIcosahedron(float radius) {
	const float X = radius;
	float t = (1.0 + sqrt(5.0)) / 2.0; // Golden ratio
	t *= radius;

	Vertex vertices[] = {
		// Z Plane orthogonal rectangles (Vertical)
		{ XMFLOAT3(-X, t, 0), XMFLOAT3(), XMFLOAT2() },
		{ XMFLOAT3(X, t, 0), XMFLOAT3(), XMFLOAT2() },
		{ XMFLOAT3(-X,-t, 0), XMFLOAT3(), XMFLOAT2() },
		{ XMFLOAT3(X,-t, 0), XMFLOAT3(), XMFLOAT2() },
		// X Plane orthogonal rectangles
		{ XMFLOAT3(0,-X, t), XMFLOAT3(), XMFLOAT2() },
		{ XMFLOAT3(0, X, t), XMFLOAT3(), XMFLOAT2() },
		{ XMFLOAT3(0,-X,-t), XMFLOAT3(), XMFLOAT2() },
		{ XMFLOAT3(0, X,-t), XMFLOAT3(), XMFLOAT2() },
		// Y Plane orthogonal rectangles
		{ XMFLOAT3(t, 0, -X), XMFLOAT3(), XMFLOAT2() },
		{ XMFLOAT3(t, 0,  X), XMFLOAT3(), XMFLOAT2() },
		{ XMFLOAT3(-t, 0, -X), XMFLOAT3(), XMFLOAT2() },
		{ XMFLOAT3(-t, 0,  X), XMFLOAT3(), XMFLOAT2() }
	};

	WORD indices[] = {
		// 5 faces around point 0
		0,5,11, 0,1,5, 0,7,1, 0,10,7, 0,11,10,
		// 5 adjacent faces
		1,9,5, 5,4,11, 11,2,10, 10,6,7, 7,8,1,
		// 5 faces around point 3
		3,4,9, 3,2,4, 3,6,2, 3,8,6, 3,9,8,
		// 5 adjacent faces
		4,5,9, 2,11,4, 6,10,2, 8,7,6, 9,1,8
	};

	// Compute TextCoord
	const float invHalfPI = 1 / XM_PI * 0.5;
	const float invPI = 1 / XM_PI;
	for (int i = 0; i < ARRAYSIZE(vertices); i++) {
		XMFLOAT3 pos = vertices[i].position;
		XMVECTOR posNorm = XMVector3Normalize((XMVectorSet(pos.x, pos.y, pos.z, 1.0f)));
		vertices[i].texCoord.x = (atan2(XMVectorGetX(posNorm), XMVectorGetZ(posNorm)) + XM_PI) * invHalfPI;
		vertices[i].texCoord.y = (acos(XMVectorGetY(posNorm)) + XM_PI) * invPI;
	}

	// Create Icosahedron
	comp::VertexBuffer vertexBuffer = m_ctx.rcommand->CreateVertexBuffer(vertices, ARRAYSIZE(vertices), sizeof(Vertex));
	comp::IndexBuffer indexBuffer = m_ctx.rcommand->CreateIndexBuffer(indices, ARRAYSIZE(indices));

	// Store data
	comp::Mesh mesh = {};
	mesh.ib = indexBuffer;
	mesh.vb = vertexBuffer;
	return mesh;
}
