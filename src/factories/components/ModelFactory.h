#pragma once

#include <GLTFSDK/GLTF.h>
#include <GLTFSDK/GLTFResourceReader.h>
#include <GLTFSDK/GLBResourceReader.h>
#include <GLTFSDK/Deserialize.h>

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

private:
	Microsoft::glTF::Document ReadGltf(std::filesystem::path gltfFilePat);
};

/**
 * @brief
 */
class StreamReader : public Microsoft::glTF::IStreamReader {
public:
	/**
	 * @param basePath - The absolute path (without the filename)
	 */
	StreamReader(std::filesystem::path basePath);

	/**
	 * @brief Resolves the relative URIs of any external resources declared in the glTF manifest
	 */
	std::shared_ptr<std::istream> GetInputStream(const std::string& filename) const override;

private:
	std::filesystem::path m_pathBase;
};
