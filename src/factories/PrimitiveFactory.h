#pragma once

#include "core/Context.h"

class PrimitiveFactory {
public:
	PrimitiveFactory(Context& context);
	~PrimitiveFactory();

	struct Vertex {
		XMFLOAT3 position;
		XMFLOAT3 normal;
		XMFLOAT2 texCoord;
	};

	/**
	 * @brief Proceduraly generated UV Sphere
	 * @link http://www.songho.ca/opengl/gl_sphere.html
	 */
	unsigned int CreateUVSphere(float radius = 1, float sectorCount = 14, float stackCount = 28);

	/**
	 * @brief Proceduraly generated ICO Sphere
	 * @link https://www.youtube.com/watch?v=4u7HXv4b5-U
	 */
	unsigned int CreateIcoSphere(float radius = 1, unsigned int subdivisionCount = 3);

	/**
	 * @brief Proceduraly generated Box
	 */
	unsigned int CreateBox(float width = 1, float height = 1);

	/**
	 * @brief Proceduraly generated Icosahedron
	 * @link http://blog.andreaskahler.com/2009/06/creating-icosphere-mesh-in-code.html
	 */
	unsigned int CreateIcosahedron(float radius = 1);

	D3D11_INPUT_ELEMENT_DESC* GetIed() { return m_ied.data(); }
	unsigned int GetIedElementCount() { return m_ied.size(); }
	unsigned int GetIedByteWidth() { return sizeof(m_ied.data()); }

private:
	std::array<D3D11_INPUT_ELEMENT_DESC, 3> m_ied;
	Context& m_ctx;
};
