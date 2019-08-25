#pragma once

namespace prim {
	struct Vertex {
		XMFLOAT3 position;
		XMFLOAT3 normal;
		XMFLOAT2 texCoord;
	};
	static D3D11_INPUT_ELEMENT_DESC InputElements[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	static const unsigned int InputElementCount = ARRAYSIZE(InputElements);
	static const unsigned int InputElementSize = sizeof(Vertex);

	/**
	 * @brief Common interface to every Geometric Primitive
	 */
	class IPrimitive {
	public:
		virtual ~IPrimitive() {};

		std::vector<Vertex> GetVertices() const { return m_vertices; }
		std::vector<WORD> GetIndices() const { return m_indices; }

	protected:
		std::vector<Vertex> m_vertices;
		std::vector<WORD> m_indices;
	};

	/**
	 * @brief Proceduraly generated UV Sphere
	 * @link http://www.songho.ca/opengl/gl_sphere.html
	 */
	class UVSphere : public IPrimitive {
	public:
		UVSphere(float radius = 1, float sectorCount = 14, float stackCount = 28);
	};

	/**
	 * @brief Proceduraly generated ICO Sphere
	 * @link https://schneide.blog/2016/07/15/generating-an-icosphere-in-c/
	 */
	class IcoSphere : public IPrimitive {
	public:
		IcoSphere(float radius = 1, unsigned int subdivisionCount = 3);
	};

	/**
	 * @brief Proceduraly generated Box
	 */
	class Box : public IPrimitive {
	public:
		Box(float width = 1, float height = 1);
	};

	/**
	 * @brief Proceduraly generated Icosahedron
	 * @link http://blog.andreaskahler.com/2009/06/creating-icosphere-mesh-in-code.html
	 */
	class Icosahedron : public IPrimitive {
	public:
		Icosahedron(float radius = 1);
	};
};
