#pragma once

#include "components/graphics/Model.h"

#include <fx/gltf.h>

/**
 * @brief
 */
class ModelFactory {
public:
	ModelFactory();
	~ModelFactory();

	struct GltfBufferInfo {
		fx::gltf::Accessor const* accessor;
		uint8_t const* data;
		uint32_t dataStride;
		uint32_t totalSize;
	};

	struct GltfImageInfo {
		std::string fileName{};
		uint32_t binarySize{};
		uint8_t* binaryData{};
	};

	/**
	 * @brief
	 */
	comp::Model CreateModel(std::filesystem::path gltfFilePath);

private:
	GltfBufferInfo GetData(fx::gltf::Document const& doc, fx::gltf::Accessor const& accessor);

	uint32_t CalculateDataTypeSize(fx::gltf::Accessor const& accessor);
};
