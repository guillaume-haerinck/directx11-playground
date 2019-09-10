#pragma once

#include "components/graphics/Shader.h"

/**
 * @brief CPU-side data for Materials.
 *		  Will be transformed to corresponding constant buffer structure (array of materials)
 *		  and sent over to GPU if there is a change.
 */
namespace scomp {

	/**
	 * @brief An image accessible by shaders
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
	 * @brief Texture slot used in shaders for phong materials
	 */
	enum PhongTexSlot {
		DIFFUSE = 0,
		SPECULAR = 1
	};

	/**
	 * @brief Structure of data for one Phong material
	 */
	struct PhongMaterial {
		std::vector<Texture> textures;
	};

	/**
	 * @brief Unique array of Phong materials used for every objects of the scene
	 *
	 * @note Get the constant buffer corresponding to Materials from the shader component.
	 *		 It will be updated in the render system if there is a change.
	 */
	struct PhongMaterials {
		std::vector<PhongMaterial> materials;
		bool hasToBeUpdated;
	};

	///////////////////////////////////////////////////////////////////////////
	////////////////////////////// COOK-TORRANCE //////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	/**
	 * @brief Stucture of data for one PBR material
	 */
	using CookTorranceMaterial = fx::gltf::Material;

	/**
	 * @brief Unique array of PBR materials used for every objects of the scene
	 *
	 * @note Get the constant buffer corresponding to Materials from the shader component.
	 *		 It will be updated in the render system if there is a change.
	 */
	struct CookTorranceMaterials {
		std::vector<CookTorranceMaterial> materials;
		bool hasToBeUpdated;
	};
}
