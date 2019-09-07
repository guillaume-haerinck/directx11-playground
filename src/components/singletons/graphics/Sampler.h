#pragma once

namespace scomp {

	enum SamplerSlot {
		ANISOTROPIC_WRAP = 0,
		LINEAR_CLAMP
	};

	/**
	 * @brief Texture sampler
	 */
	struct Sampler {
		Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler;
		unsigned int slot;
	};

	/**
	 * @brief Unique array of samplers for the whole app
	 */
	struct Samplers {
		std::vector<Sampler> samplers;
	};
}
