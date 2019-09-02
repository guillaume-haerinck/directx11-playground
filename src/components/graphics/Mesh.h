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
	// TODO  handle slot & texture array
	struct Texture {
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler;
		unsigned int slot;
	};

	/**
	 * @brief
	 */
	struct Mesh {
		Mesh(VertexBuffer vertexBuffer, IndexBuffer indexBuffer = {}, Texture* textureArray = nullptr, unsigned int textureCount = 0)
			: vb(vertexBuffer), ib(indexBuffer)
		{
			if (textureArray != nullptr && textureCount > 0) {
				textures.resize(textureCount);
				for (int i = 0; i < textureCount; i++) {
					textures.at(i) = textureArray[i];
				}
			}
		}

		VertexBuffer vb;
		IndexBuffer ib;
		std::vector<Texture> textures;
	};
}
