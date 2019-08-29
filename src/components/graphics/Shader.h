#pragma once

#include <vector>
#include <d3d11_1.h>
#include <wrl.h>

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
	 */
	struct VertexShader {
		VertexShader(ID3D11VertexShader* shader, ID3D11InputLayout* layout, ConstantBuffer* CBufferArray = nullptr, unsigned int CBufferCount = 0) 
			: shader(shader), layout(layout)
		{
			if (CBufferArray != nullptr && CBufferCount > 0) {
				constantBuffers.resize(CBufferCount);
				for (int i = 0; i < CBufferCount; i++) {
					constantBuffers.at(i) = CBufferArray[i];
				}
			}
		}

		Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> layout;
		std::vector<ConstantBuffer> constantBuffers;
	};

	/**
	 * @brief
	 */
	struct PixelShader {
		PixelShader(ID3D11PixelShader* shader, ConstantBuffer* CBufferArray = nullptr, unsigned int CBufferCount = 0)
			: shader(shader)
		{
			if (CBufferArray != nullptr && CBufferCount > 0) {
				constantBuffers.resize(CBufferCount);
				for (int i = 0; i < CBufferCount; i++) {
					constantBuffers.at(i) = CBufferArray[i];
				}
			}
		}

		Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;
		std::vector<ConstantBuffer> constantBuffers;
	};
}
