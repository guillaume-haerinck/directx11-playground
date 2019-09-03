#pragma once

#include "components/graphics/Model.h"
#include "components/graphics/Mesh.h"
#include "core/Context.h"

#include <fx/gltf.h>

/**
 * @brief
 */
class ModelFactory {
public:
	ModelFactory(Context& context);
	~ModelFactory();

	struct GltfBufferInfo {
		fx::gltf::Accessor const* accessor;
		uint8_t const* data;
		uint32_t dataStride;
		uint32_t totalSize;
		uint32_t vertexCount;
	};

	struct GltfImageInfo {
		std::string fileName{};
		uint32_t binarySize{};
		uint8_t* binaryData{};
	};

	/**
	 * @brief
	 */
	comp::Model CreateModel(const char* gltfFilePath);

private:
	GltfBufferInfo GetData(fx::gltf::Document const& doc, fx::gltf::Accessor const& accessor);

	uint32_t CalculateDataTypeSize(fx::gltf::Accessor const& accessor);

private:
	Context& m_ctx;
};
