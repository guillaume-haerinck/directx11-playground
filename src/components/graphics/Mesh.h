#pragma once

#include <vector>
#include <d3d11_1.h>
#include <wrl.h>

namespace comp {
	/**
	 * @brief
	 */
	struct Texture {
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler;
	};

	/**
	 * @brief
	 */
	struct Mesh {
		Mesh(ID3D11Buffer* vertexBuffer, unsigned int vertexBufferStride, unsigned int vertexBufferCount,
			ID3D11Buffer* indexBuffer = nullptr, unsigned int indexBufferCount = 0, 
			Texture* textureArray = nullptr, unsigned int textureCount = 0) 
			: vertexBuffer(vertexBuffer), VBStride(vertexBufferStride), VBCount(vertexBufferCount),
			  indexBuffer(indexBuffer), IBCount(indexBufferCount)
		{
			if (textureArray != nullptr && textureCount > 0) {
				textures.resize(textureCount);
				for (int i = 0; i < textureCount; i++) {
					textures.at(i) = textureArray[i];
				}
			}
		}

		Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
		unsigned int VBStride;
		unsigned int VBCount;
		Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
		unsigned int IBCount;
		std::vector<Texture> textures;
	};
}
