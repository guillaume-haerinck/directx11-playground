#pragma once

namespace comp {
	/**
	 * @brief Constant buffers used by shader. This structure is used to update constant buffers.
	 */
	struct ConstantBuffer {
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
		unsigned int byteWidth;
		// TODO keep track of the layout
	};

	/**
	 * @brief A vertex shader. The first stage in the graphic pipeline.
	 *
	 * @note Constant buffers will be bound but not updated by this component.
	 *		 The slot will correspond to the index of the vector the constant buffer is in.
	 */
	struct VertexShader {
		Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> layout;
		std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> constantBuffers;
	};

	/**
	 * @brief A pixel shader. The last stage in the graphic pipeline.
	 *
	 * @note Constant buffers will be bound but not updated by this component.
	 *		 The slot will correspond to the index of the vector the constant buffer is in.
	 */
	struct PixelShader {
		Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;
		std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> constantBuffers;
	};
}
