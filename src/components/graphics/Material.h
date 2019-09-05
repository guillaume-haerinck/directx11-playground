#pragma once

namespace comp {

	/**
	 * @brief
	 */
	struct Texture {
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
		unsigned int samplerSlot;
		unsigned int slot;
	};

	///////////////////////////////////////////////////////////////////////////
	//////////////////////////////// BLINN-PHONG //////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	/**
	 * @brief
	 */
	enum PhongTexSlot {
		DIFFUSE = 0
	};

	/**
	 * @brief
	 */
	struct PhongMaterial {
		std::vector<Texture> textures;
	};

	/**
	 * @brief
	 */
	struct PhongMaterials {
		std::vector<PhongMaterial> materials;
	};

	///////////////////////////////////////////////////////////////////////////
	////////////////////////////// COOK-TORRANCE //////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	/**
	 * @brief
	 */
	using CookTorranceMaterial = fx::gltf::Material;

	/**
	 * @brief
	 */
	struct CookTorranceMaterials {
		std::vector<CookTorranceMaterial> materials;
	};
}
