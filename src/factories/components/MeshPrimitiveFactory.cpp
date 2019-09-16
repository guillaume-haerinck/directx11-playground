#include "pch.h"
#include "MeshPrimitiveFactory.h"

#include "components/graphics/Mesh.h"

MeshPrimitiveFactory::MeshPrimitiveFactory(Context& context) : m_ctx(context) {
	m_ied.at(0) = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	m_ied.at(1) = { "NORMAL",	0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	m_ied.at(2) = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
}

MeshPrimitiveFactory::~MeshPrimitiveFactory()
{
}

comp::Mesh MeshPrimitiveFactory::CreateUVSphere(float radius, float sectorCount, float stackCount) {
	float x, y, z, xy;								// vertex position
	float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
	float s, t;                                     // vertex texCoord

	float sectorStep = 2 * DX::XM_PI / sectorCount;
	float stackStep = DX::XM_PI / stackCount;
	float sectorAngle, stackAngle;

	// Vertex attributes
	std::vector<DX::XMFLOAT3> positions;
	std::vector<DX::XMFLOAT3> normals;
	std::vector<DX::XMFLOAT2> texCoords;

	for (int i = 0; i <= stackCount; ++i) {
		stackAngle = DX::XM_PIDIV2 - i * stackStep;     // starting from pi/2 to -pi/2
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

			// Add to buffers
			positions.push_back(DX::XMFLOAT3(x, y, z));
			normals.push_back(DX::XMFLOAT3(nx, ny, nz));
			texCoords.push_back(DX::XMFLOAT2(s, t));
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

	// Create buffers
	comp::AttributeBuffer positionBuffer = m_ctx.rcommand->CreateAttributeBuffer(positions.data(), positions.size(), sizeof(DX::XMFLOAT3));
	comp::AttributeBuffer normalBuffer = m_ctx.rcommand->CreateAttributeBuffer(normals.data(), normals.size(), sizeof(DX::XMFLOAT3));
	comp::AttributeBuffer texCoordBuffer = m_ctx.rcommand->CreateAttributeBuffer(texCoords.data(), texCoords.size(), sizeof(DX::XMFLOAT2));
	comp::IndexBuffer ib = m_ctx.rcommand->CreateIndexBuffer(indices.data(), indices.size());

	// Store buffers
	comp::VertexBuffer vb = {};
	vb.buffers = { positionBuffer.buffer, normalBuffer.buffer, texCoordBuffer.buffer };
	vb.byteWidths = { positionBuffer.byteWidth, normalBuffer.byteWidth, texCoordBuffer.byteWidth };
	vb.counts = { positionBuffer.count, normalBuffer.count, texCoordBuffer.count };
	vb.strides = { positionBuffer.stride, normalBuffer.stride, texCoordBuffer.stride };
	vb.offsets = { 0, 0, 0 };
	vb.names = { "position", "normal", "texture coordinates" };

	// Send result
	comp::Mesh mesh = {};
	mesh.ib = ib;
	mesh.vb = vb;
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
	DX::XMFLOAT3 positions[] = {
		// Front v0,v1,v2,v3
		DX::XMFLOAT3(1, 1, 1), DX::XMFLOAT3(-1, 1, 1), DX::XMFLOAT3(-1, -1, 1), DX::XMFLOAT3(1, -1, 1),
		// Right v0,v3,v4,v5
		DX::XMFLOAT3(1, 1, 1), DX::XMFLOAT3(1, -1, 1), DX::XMFLOAT3(1, -1, -1), DX::XMFLOAT3(1, 1, -1),
		// Top v0,v5,v6,v1	
		DX::XMFLOAT3(1, 1, 1), DX::XMFLOAT3(1, 1, -1), DX::XMFLOAT3(-1, 1, -1), DX::XMFLOAT3(-1, 1, 1), 
		// Left v1,v6,v7,v2	
		DX::XMFLOAT3(-1, 1, 1), DX::XMFLOAT3(-1, 1, -1), DX::XMFLOAT3(-1, -1, -1), DX::XMFLOAT3(-1, -1, 1),  
		// Bottom v7,v4,v3,v2
		DX::XMFLOAT3(-1, -1, -1), DX::XMFLOAT3(1, -1, -1), DX::XMFLOAT3(1, -1, 1), DX::XMFLOAT3(-1, -1, 1), 
		// Back v4,v7,v6,v5	
		DX::XMFLOAT3(1, -1, -1), DX::XMFLOAT3(-1, -1, -1), DX::XMFLOAT3(-1, 1, -1), DX::XMFLOAT3(1, 1, -1)
	};
	
	DX::XMFLOAT3 normals[] = {
		DX::XMFLOAT3(0, 0, 1), DX::XMFLOAT3(0, 0, 1), DX::XMFLOAT3(0, 0, 1), DX::XMFLOAT3(0, 0, 1),
		DX::XMFLOAT3(1, 0, 0), DX::XMFLOAT3(1, 0, 0), DX::XMFLOAT3(1, 0, 0), DX::XMFLOAT3(1, 0, 0),
		DX::XMFLOAT3(0, 1, 0), DX::XMFLOAT3(0, 1, 0), DX::XMFLOAT3(0, 1, 0), DX::XMFLOAT3(0, 1, 0),
		DX::XMFLOAT3(-1, 0, 0), DX::XMFLOAT3(-1, 0, 0), DX::XMFLOAT3(-1, 0, 0), DX::XMFLOAT3(-1, 0, 0),
		DX::XMFLOAT3(0,-1, 0), DX::XMFLOAT3(0,-1, 0), DX::XMFLOAT3(0,-1, 0), DX::XMFLOAT3(0,-1, 0),
		DX::XMFLOAT3(0, 0,-1), DX::XMFLOAT3(0, 0,-1), DX::XMFLOAT3(0, 0,-1), DX::XMFLOAT3(0, 0,-1)
	};

	DX::XMFLOAT2 texCoords[] = {
		DX::XMFLOAT2(1, 0), DX::XMFLOAT2(0, 0), DX::XMFLOAT2(0, 1), DX::XMFLOAT2(1, 1),
		DX::XMFLOAT2(0, 0), DX::XMFLOAT2(0, 1), DX::XMFLOAT2(1, 1), DX::XMFLOAT2(1, 0),
		DX::XMFLOAT2(1, 1), DX::XMFLOAT2(1, 0), DX::XMFLOAT2(0, 0), DX::XMFLOAT2(0, 1),
		DX::XMFLOAT2(1, 0), DX::XMFLOAT2(0, 0), DX::XMFLOAT2(0, 1), DX::XMFLOAT2(1, 1),
		DX::XMFLOAT2(0, 1), DX::XMFLOAT2(1, 1), DX::XMFLOAT2(1, 0), DX::XMFLOAT2(0, 0),
		DX::XMFLOAT2(0, 1), DX::XMFLOAT2(1, 1), DX::XMFLOAT2(1, 0), DX::XMFLOAT2(0, 0)
	};

	WORD indices[] = {
		0, 1, 2,   2, 3, 0,       // front
		4, 5, 6,   6, 7, 4,       // right
		8, 9, 10,  10,11, 8,      // top
		12,13,14,  14,15,12,      // left
		16,17,18,  18,19,16,      // bottom
		20,21,22,  22,23,20		  // back
	};

	// Create buffers
	comp::AttributeBuffer positionBuffer = m_ctx.rcommand->CreateAttributeBuffer(positions, ARRAYSIZE(positions), sizeof(DX::XMFLOAT3));
	comp::AttributeBuffer normalBuffer = m_ctx.rcommand->CreateAttributeBuffer(normals, ARRAYSIZE(normals), sizeof(DX::XMFLOAT3));
	comp::AttributeBuffer texCoordBuffer = m_ctx.rcommand->CreateAttributeBuffer(texCoords, ARRAYSIZE(texCoords), sizeof(DX::XMFLOAT2));
	comp::IndexBuffer ib = m_ctx.rcommand->CreateIndexBuffer(indices, ARRAYSIZE(indices));

	// Store buffers
	comp::VertexBuffer vb = {};
	vb.buffers = { positionBuffer.buffer, normalBuffer.buffer, texCoordBuffer.buffer };
	vb.byteWidths = { positionBuffer.byteWidth, normalBuffer.byteWidth, texCoordBuffer.byteWidth };
	vb.counts = { positionBuffer.count, normalBuffer.count, texCoordBuffer.count };
	vb.strides = { positionBuffer.stride, normalBuffer.stride, texCoordBuffer.stride };
	vb.offsets = { 0, 0, 0 };
	vb.names = { "position", "normal", "texture coordinates" };

	// Send result
	comp::Mesh mesh = {};
	mesh.ib = ib;
	mesh.vb = vb;
	return mesh;
}

comp::Mesh MeshPrimitiveFactory::CreateIcosahedron(float radius) {
	const float X = radius;
	float t = (1.0 + sqrt(5.0)) / 2.0; // Golden ratio
	t *= radius;

	DX::XMFLOAT3 positions[] = {
		// Z Plane orthogonal rectangles (Vertical)
		DX::XMFLOAT3(-X, t, 0), DX::XMFLOAT3(X, t, 0), DX::XMFLOAT3(-X,-t, 0), DX::XMFLOAT3(X,-t, 0),
		// X Plane orthogonal rectangles
		DX::XMFLOAT3(0,-X, t), DX::XMFLOAT3(0, X, t), DX::XMFLOAT3(0,-X,-t), DX::XMFLOAT3(0, X,-t),
		// Y Plane orthogonal rectangles
		DX::XMFLOAT3(t, 0, -X), DX::XMFLOAT3(t, 0,  X), DX::XMFLOAT3(-t, 0, -X), DX::XMFLOAT3(-t, 0,  X),
	};

	// TODO
	DX::XMFLOAT3 normals[] = {
		DX::XMFLOAT3(), DX::XMFLOAT3(), DX::XMFLOAT3(), DX::XMFLOAT3(),
		DX::XMFLOAT3(), DX::XMFLOAT3(), DX::XMFLOAT3(), DX::XMFLOAT3(),
		DX::XMFLOAT3(), DX::XMFLOAT3(), DX::XMFLOAT3(), DX::XMFLOAT3()
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
	const float invHalfPI = 1 / DX::XM_PI * 0.5;
	const float invPI = 1 / DX::XM_PI;
	std::vector<DX::XMFLOAT2> texCoords;
	for (int i = 0; i < ARRAYSIZE(positions); i++) {
		DX::XMFLOAT3 pos = positions[i];
		DX::XMVECTOR posNorm = DX::XMVector3Normalize((DX::XMVectorSet(pos.x, pos.y, pos.z, 1.0f)));
		float x = (atan2(DX::XMVectorGetX(posNorm), DX::XMVectorGetZ(posNorm)) + DX::XM_PI) * invHalfPI;
		float y = (acos(DX::XMVectorGetY(posNorm)) + DX::XM_PI) * invPI;
		texCoords.push_back(DX::XMFLOAT2(x, y));
	}

	// Create buffers
	comp::AttributeBuffer positionBuffer = m_ctx.rcommand->CreateAttributeBuffer(positions, ARRAYSIZE(positions), sizeof(DX::XMFLOAT3));
	comp::AttributeBuffer normalBuffer = m_ctx.rcommand->CreateAttributeBuffer(normals, ARRAYSIZE(normals), sizeof(DX::XMFLOAT3));
	comp::AttributeBuffer texCoordBuffer = m_ctx.rcommand->CreateAttributeBuffer(texCoords.data(), texCoords.size(), sizeof(DX::XMFLOAT2));
	comp::IndexBuffer ib = m_ctx.rcommand->CreateIndexBuffer(indices, ARRAYSIZE(indices));

	// Store buffers
	comp::VertexBuffer vb = {};
	vb.buffers = { positionBuffer.buffer, normalBuffer.buffer, texCoordBuffer.buffer };
	vb.byteWidths = { positionBuffer.byteWidth, normalBuffer.byteWidth, texCoordBuffer.byteWidth };
	vb.counts = { positionBuffer.count, normalBuffer.count, texCoordBuffer.count };
	vb.strides = { positionBuffer.stride, normalBuffer.stride, texCoordBuffer.stride };
	vb.offsets = { 0, 0, 0 };
	vb.names = { "position", "normal", "texture coordinates" };

	// Send result
	comp::Mesh mesh = {};
	mesh.ib = ib;
	mesh.vb = vb;
	return mesh;
}
