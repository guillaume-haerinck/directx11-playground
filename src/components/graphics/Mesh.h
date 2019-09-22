#pragma once

#include "scomponents/graphics/Materials.h"

namespace comp {
	/**
	 * @brief Vertex attribute buffer (points, uv mapping, normals, etc...)
	 */
	struct AttributeBuffer {
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
		unsigned int stride = 0;
		unsigned int count = 0;
		unsigned int byteWidth = 0;
	};

	/**
	 * @brief Keep references to vertex attributes sent to GPU
	 *
	 * @note Attributes are stored in separate buffers (PPP)(TTT)(NNN).
	 *		 This instead of interleaved (PTNPTNPTN) or packed (PPPTTTNNN).
	 */
	struct VertexBuffer {
		std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> buffers;
		std::vector<unsigned int> strides;
		std::vector<unsigned int> counts;
		std::vector<unsigned int> byteWidths;
		std::vector<unsigned int> offsets;
		std::vector<std::string> names;
	};

	/**
	 * @brief Data used to prevent vertex duplication
	 */
	struct IndexBuffer {
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
		unsigned int count = 0;
	};

	/**
	 * @param materialIndex - The index in the std::vector of Materials component (0 if default material).
	 *						  The same material can be shared by multiple mesh.
	 */
	struct Mesh {
		VertexBuffer vb;
		IndexBuffer ib;
		unsigned int materialIndex = 0;
		scomp::MaterialType materialType = scomp::MaterialType::NO_MATERIAL;
	};
}
