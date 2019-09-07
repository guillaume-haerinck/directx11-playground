#pragma once

namespace comp {
	/**
	 * @brief
	 */
	struct ConstantBuffer {
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
		unsigned int slot;
		unsigned int byteWidth;
		// TODO keep track of the layout
	};

	/**
	 * @brief
	 *
	 * @note Constant buffers will be bound but not updated by this component
	 */
	struct VertexShader {
		Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> layout;
		std::vector<ConstantBuffer> constantBuffers;
	};

	/**
	 * @brief
	 *
	 * @note Constant buffers will be bound but not updated by this component
	 */
	struct PixelShader {
		Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;
		std::vector<ConstantBuffer> constantBuffers;
	};
}
