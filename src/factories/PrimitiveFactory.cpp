#include "pch.h"
#include "PrimitiveFactory.h"

namespace prim {

	///////////////////////////////////////////////////////////////////////////
	//////////////////////////////// UV SPHERE ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	UVSphere::UVSphere(float radius, float sectorCount, float stackCount) {
		float x, y, z, xy;
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
				x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
				y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
				m_vertices.push_back(XMFLOAT3(x, y, z));
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

	}

	///////////////////////////////////////////////////////////////////////////
	//////////////////////////////////// BOX //////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	Box::Box(float width, float height) {
		m_vertices = {
			{ XMFLOAT3(-1.0f, -1.0f, -1.0f) }, // Front Lower left
			{ XMFLOAT3(1.0f, -1.0f, -1.0f) }, // Front Lower right	
			{ XMFLOAT3(-1.0f,  1.0f, -1.0f) }, // Front Top left
			{ XMFLOAT3(1.0f,  1.0f, -1.0f) }, // Front Top right

			{ XMFLOAT3(-1.0f, -1.0f,  1.0f) }, // Back Lower left
			{ XMFLOAT3(1.0f, -1.0f,  1.0f) }, // Back Lower right	
			{ XMFLOAT3(-1.0f,  1.0f,  1.0f) }, // Back Top left
			{ XMFLOAT3(1.0f,  1.0f,  1.0f) }  // Back Top right
		};

		/* Welding order must be clockwise
							6_______7
						   /|       |
						  / |      /|
						 /  4_____/_5
						2_______3/ /
						|       | /
						|       |/
						0_______1
		*/
		m_indices = {
			0,2,1, 2,3,1,
			1,3,5, 3,7,5,
			2,6,3, 3,6,7,
			4,5,7, 4,7,6,
			0,4,2, 2,4,6,
			0,1,4, 1,5,4
		};
	}

	///////////////////////////////////////////////////////////////////////////
	//////////////////////////////// ICOSAHEDRON //////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	Icosahedron::Icosahedron(float radius) {
		const float X = radius;
		const float Z = (1.0 + sqrt(5.0)) / 2.0;

		m_vertices = {
			XMFLOAT3(-X,0,Z), XMFLOAT3(X,0,Z), XMFLOAT3(-X,0,-Z), XMFLOAT3(X,0,-Z),
			XMFLOAT3(0,Z,X), XMFLOAT3(0,Z,-X), XMFLOAT3(0,-Z,X), XMFLOAT3(0,-Z,-X),
			XMFLOAT3(Z,X,0), XMFLOAT3(-Z,X, 0), XMFLOAT3(Z,-X,0), XMFLOAT3(-Z,-X, 0)
		};

		m_indices = {
			0,4,1,  0,9,4,  9,5,4,  4,5,8,  4,8,1,
			8,10,1, 8,3,10, 5,3,8,  5,2,3,  2,7,3,
			7,10,3, 7,6,10, 7,11,6, 11,0,6, 0,1,6,
			6,1,10, 9,0,11, 9,11,2, 9,2,5,  7,2,11
		};
	}
};
