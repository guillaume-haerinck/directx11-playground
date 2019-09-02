#pragma once

namespace comp {
	/**
	 * @brief
	 */
	struct VertexBuffer {
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
		unsigned int stride;
		unsigned int count;
		unsigned int byteWidth;
	};

	/**
	 * @brief
	 */
	struct IndexBuffer {
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
		unsigned int count;
	};

	/**
	 * @brief
	 */
	struct Mesh {
		Mesh(VertexBuffer vb, IndexBuffer ib = {}) : vb(vb), ib(ib) {}

		VertexBuffer vb;
		IndexBuffer ib;
	};

	/**
	 * @brief
	 */
	struct Meshes {
		std::vector<Mesh> meshes;
	};
}
