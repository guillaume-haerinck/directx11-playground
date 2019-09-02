#pragma once

#include "components/graphics/Model.h"

/**
 * @brief
 */
class ModelFactory {
public:
	ModelFactory();
	~ModelFactory();

	/**
	 * @brief
	 */
	comp::Model CreateModel(std::filesystem::path gltfFilePat);
};
