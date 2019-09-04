#pragma once

namespace comp {
	/**
	 * @brief Vertex attribute buffer
	 */
	struct AttributeBuffer {
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
		unsigned int stride;
		unsigned int count;
		unsigned int byteWidth;
	};

	/**
	 * @brief 
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
		VertexBuffer vb;
		IndexBuffer ib;
	};
}
