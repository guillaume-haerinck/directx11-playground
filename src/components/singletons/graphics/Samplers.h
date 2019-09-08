#pragma once

namespace scomp {

	/**
	 * @brief Slot used by shader to identify the sampler
	 */
	enum SamplerSlot {
		ANISOTROPIC_WRAP = 0,
		LINEAR_CLAMP = 1
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
