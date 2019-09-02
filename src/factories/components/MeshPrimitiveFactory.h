#pragma once

#include "core/Context.h"

/**
 * @brief Creates primitive Mesh components
 */
class MeshPrimitiveFactory {
public:
	MeshPrimitiveFactory(Context& context);
	~MeshPrimitiveFactory();

	struct Vertex {
		XMFLOAT3 position;
		XMFLOAT3 normal;
		XMFLOAT2 texCoord;
	};

	/**
	 * @brief Creates UV Sphere Mesh component
	 * @link http://www.songho.ca/opengl/gl_sphere.html
	 */
	comp::Mesh CreateUVSphere(float radius = 1, float sectorCount = 14, float stackCount = 28);

	/**
	 * @brief Creates ICO Sphere Mesh component
	 * @link https://www.youtube.com/watch?v=4u7HXv4b5-U
	 */
	comp::Mesh CreateIcoSphere(float radius = 1, unsigned int subdivisionCount = 3);

	/**
	 * @brief Creates a Box Mesh component
	 */
	comp::Mesh CreateBox(float width = 1, float height = 1);

	/**
	 * @brief Creates Icosahedron Mesh component
	 * @link http://blog.andreaskahler.com/2009/06/creating-icosphere-mesh-in-code.html
	 */
	comp::Mesh CreateIcosahedron(float radius = 1);

	D3D11_INPUT_ELEMENT_DESC* GetIed() { return m_ied.data(); }
	unsigned int GetIedElementCount() { return m_ied.size(); }
	unsigned int GetIedByteWidth() { return sizeof(m_ied.data()); }

private:
	std::array<D3D11_INPUT_ELEMENT_DESC, 3> m_ied;
	Context& m_ctx;
};
