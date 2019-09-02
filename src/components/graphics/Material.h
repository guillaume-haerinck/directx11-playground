#pragma once

namespace comp {
	/**
	 * @brief
	 */
	struct Texture {
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler;
		unsigned int slot;
	};

	enum PBRTexSlot {
		BASIC_COLOR = 0
	};

	/**
	 * @brief
	 */
	struct PBRMaterial {
		std::vector<Texture> textures;
	};

	enum PhongTexSlot {
		DIFFUSE = 0
	};

	/**
	 * @brief
	 */
	struct PhongMaterial {
		std::vector<Texture> textures;
	};
}