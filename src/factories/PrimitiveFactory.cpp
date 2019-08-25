#include "pch.h"
#include "PrimitiveFactory.h"

namespace prim {

	///////////////////////////////////////////////////////////////////////////
	//////////////////////////////// UV SPHERE ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	UVSphere::UVSphere(float radius, float sectorCount, float stackCount) {
		float x, y, z, xy;								// vertex position
		float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
		float s, t;                                     // vertex texCoord
		
		float sectorStep = 2 * XM_PI / sectorCount;
		float stackStep = XM_PI / stackCount;
		float sectorAngle, stackAngle;

		// Vertices
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

				m_vertices.push_back({ XMFLOAT3(x, y, z), XMFLOAT3(nx, ny, nz), XMFLOAT2(s, t) });
			}
		}

		// Indices
		int k1, k2;
		for (int i = 0; i < stackCount; ++i) {
			k1 = i * (sectorCount + 1);     // beginning of current stack
			k2 = k1 + sectorCount + 1;      // beginning of next stack

			for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
				// 2 triangles per sector excluding first and last stacks
				if (i != 0) {
					m_indices.push_back(k1);
					m_indices.push_back(k1 + 1);
					m_indices.push_back(k2);
				}

				if (i != (stackCount - 1)) {
					m_indices.push_back(k1 + 1);
					m_indices.push_back(k2 + 1);
					m_indices.push_back(k2);
				}
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////
	//////////////////////////////// ICO SPHERE ///////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	IcoSphere::IcoSphere(float radius, unsigned int subdivisionCount) {
		Icosahedron icasahedron(radius);

		// TODO subdivide
	}

	///////////////////////////////////////////////////////////////////////////
	//////////////////////////////////// BOX //////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	Box::Box(float width, float height) {
		//    v6----- v5
		//   /|      /|
		//  v1------v0|
		//  | |     | |
		//  | |v7---|-|v4
		//  |/      |/
		//  v2------v3
		m_vertices = {
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

		m_indices = {  
			0, 1, 2,   2, 3, 0,       // front
			4, 5, 6,   6, 7, 4,       // right
			8, 9, 10,  10,11, 8,      // top
			12,13,14,  14,15,12,      // left
			16,17,18,  18,19,16,      // bottom
			20,21,22,  22,23,20		  // back
		};
	}

	///////////////////////////////////////////////////////////////////////////
	//////////////////////////////// ICOSAHEDRON //////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	Icosahedron::Icosahedron(float radius) {
		const float X = radius;
		const float Z = (1.0 + sqrt(5.0)) / 2.0; // Golden ratio

		m_vertices = {
			{ XMFLOAT3(-X,0,Z),  XMFLOAT3(), XMFLOAT2() },
			{ XMFLOAT3(X,0,Z), 	 XMFLOAT3(), XMFLOAT2() },
			{ XMFLOAT3(-X,0,-Z), XMFLOAT3(), XMFLOAT2() }, 
			{ XMFLOAT3(X,0,-Z),	 XMFLOAT3(), XMFLOAT2() },
			{ XMFLOAT3(0,Z,X), 	 XMFLOAT3(), XMFLOAT2() },
			{ XMFLOAT3(0,Z,-X),  XMFLOAT3(), XMFLOAT2() },
			{ XMFLOAT3(0,-Z,X),  XMFLOAT3(), XMFLOAT2() },
			{ XMFLOAT3(0,-Z,-X), XMFLOAT3(), XMFLOAT2() },
			{ XMFLOAT3(Z,X,0), 	 XMFLOAT3(), XMFLOAT2() },
			{ XMFLOAT3(-Z,X, 0), XMFLOAT3(), XMFLOAT2() }, 
			{ XMFLOAT3(Z,-X,0),  XMFLOAT3(), XMFLOAT2() },
			{ XMFLOAT3(-Z,-X, 0),XMFLOAT3(), XMFLOAT2() }
		};

		m_indices = {
			0,4,1,  0,9,4,  9,5,4,  4,5,8,  4,8,1,
			8,10,1, 8,3,10, 5,3,8,  5,2,3,  2,7,3,
			7,10,3, 7,6,10, 7,11,6, 11,0,6, 0,1,6,
			6,1,10, 9,0,11, 9,11,2, 9,2,5,  7,2,11
		};
	}
};
