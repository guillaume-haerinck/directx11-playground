#pragma once

#include "components/graphics/Mesh.h"
#include "core/Context.h"

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
	std::vector<unsigned int> CreateEntitiesFromGltf(std::filesystem::path gltfFilePath);

	D3D11_INPUT_ELEMENT_DESC* GetIed() { return m_ied.data(); }
	unsigned int GetIedElementCount() { return m_ied.size(); }
	unsigned int GetIedByteWidth() { return sizeof(m_ied.data()); }

private:
	GltfBufferInfo GetGltfBufferInfo(fx::gltf::Document const& doc, fx::gltf::Accessor const& accessor);

	uint32_t CalculateDataTypeSize(fx::gltf::Accessor const& accessor);

private:
	std::array<D3D11_INPUT_ELEMENT_DESC, 3> m_ied;
	Context& m_ctx;
};
