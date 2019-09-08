#pragma once

namespace comp {
	/**
	 * @brief Vertex attribute buffer (points, uv mapping, normals, etc...)
	 */
	struct AttributeBuffer {
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
		unsigned int stride;
		unsigned int count;
		unsigned int byteWidth;
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
		unsigned int count;
	};

	/**
	 * @param materialIndex - The index in the std::vector of Materials component (0 if default material).
	 *						  The material can be of any type (pbr, phong, toon, etc...).
	 *						  The same material can be shared by multiple mesh.
	 *
	 * @param textures - An array of textures that will be bound with the mesh.
	 *                   The texture slot will correspond to the index in the vector.
	 *                   The meaning of the texture slot depends on the material.
	 */
	struct Mesh {
		VertexBuffer vb;
		IndexBuffer ib;
		unsigned int materialIndex;
		std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> textures;
	};
}
