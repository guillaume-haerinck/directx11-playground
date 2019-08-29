#include "pch.h"
#include "PrimitiveFactory.h"

#include "components/graphics/Mesh.h"

PrimitiveFactory::PrimitiveFactory(Context& context) : m_ctx(context) {
	m_ied.at(0) = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	m_ied.at(1) = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	m_ied.at(2) = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
}

PrimitiveFactory::~PrimitiveFactory()
{
}

unsigned int PrimitiveFactory::CreateUVSphere(float radius, float sectorCount, float stackCount) {
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
				indices.push_back(k1 + 1);
				indices.push_back(k2);
			}

			if (i != (stackCount - 1)) {
				indices.push_back(k1 + 1);
				indices.push_back(k2 + 1);
				indices.push_back(k2);
			}
		}
	}

	// Create an entity with data
	return 0;
}

unsigned int PrimitiveFactory::CreateIcoSphere(float radius, unsigned int subdivisionCount) {
	return 0;
}

unsigned int PrimitiveFactory::CreateBox(float width, float height) {
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
		0, 1, 2,   2, 3, 0,       // front
		4, 5, 6,   6, 7, 4,       // right
		8, 9, 10,  10,11, 8,      // top
		12,13,14,  14,15,12,      // left
		16,17,18,  18,19,16,      // bottom
		20,21,22,  22,23,20		  // back
	};

	// Create Box
	comp::VertexBuffer vertexBuffer = m_ctx.rcommand->CreateVertexBuffer(vertices, ARRAYSIZE(vertices), sizeof(Vertex));
	comp::IndexBuffer indexBuffer = m_ctx.rcommand->CreateIndexBuffer(indices, ARRAYSIZE(indices));

	// Store data to an entity
	auto entity = m_ctx.registry.create();
	m_ctx.registry.assign<comp::Mesh>(entity, vertexBuffer, indexBuffer);

	return entity;
}

unsigned int PrimitiveFactory::CreateIcosahedron(float radius) {
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
		0,11,5,	0,5,1,  0,1,7,   0,7,10, 0,10,11,
		// 5 adjacent faces
		1,5,9,  5,11,4, 11,10,2, 10,7,6, 7,1,8,
		// 5 faces around point 3
		3,9,4,  3,4,2,  3,2,6,   3,6,8,  3,8,9,
		// 5 adjacent faces
		4,9,5,  2,4,11, 6,2,10,  8,6,7,  9,8,1
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

	// TODO compute normals from triangles
	return 0;
}
