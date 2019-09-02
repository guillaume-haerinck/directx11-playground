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
		Mesh(VertexBuffer vb = { nullptr, 0, 0, 0 }, IndexBuffer ib = { nullptr, 0 }) : vb(vb), ib(ib) {}

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
