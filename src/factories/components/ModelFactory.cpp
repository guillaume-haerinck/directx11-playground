#include "pch.h"
#include "ModelFactory.h"

///////////////////////////////////////////////////////////////////////////
////////////////////////////// MODEL FACTORY //////////////////////////////
///////////////////////////////////////////////////////////////////////////

ModelFactory::ModelFactory() {
	
}

ModelFactory::~ModelFactory() {
}

comp::Model ModelFactory::CreateModel(std::filesystem::path gltfFilePath) {
	auto document = ReadGltf(gltfFilePath);
	document;

	return comp::Model();
}

Microsoft::glTF::Document ModelFactory::ReadGltf(std::filesystem::path gltfFilePath) {
	// Create absolute from relative path
	auto gltfAbsPath = std::filesystem::current_path();
	gltfAbsPath /= gltfFilePath;
	gltfFilePath = gltfAbsPath;

	// Pass the absolute path, without the filename, to the stream reader
	auto streamReader = std::make_unique<StreamReader>(gltfFilePath.parent_path());
	std::filesystem::path pathFile = gltfFilePath.filename();
	std::filesystem::path pathFileExt = pathFile.extension();
	std::string manifest;
	auto MakePathExt = [](const std::string& ext) {
		return "." + ext;
	};

	std::unique_ptr<Microsoft::glTF::GLTFResourceReader> resourceReader;

	// If the file has a '.gltf' extension then create a GLTFResourceReader
	if (pathFileExt == MakePathExt(Microsoft::glTF::GLTF_EXTENSION)) {
		auto gltfStream = streamReader->GetInputStream(pathFile.u8string()); // Pass a UTF-8 encoded filename to GetInputString
		auto gltfResourceReader = std::make_unique<Microsoft::glTF::GLTFResourceReader>(std::move(streamReader));
		std::stringstream manifestStream;

		// Read the contents of the glTF file into a string using a std::stringstream
		manifestStream << gltfStream->rdbuf();
		manifest = manifestStream.str();
		resourceReader = std::move(gltfResourceReader);
	}

	// If the file has a '.glb' extension then create a GLBResourceReader. This class derives
	// from GLTFResourceReader and adds support for reading manifests from a GLB container's
	// JSON chunk and resource data from the binary chunk.
	if (pathFileExt == MakePathExt(Microsoft::glTF::GLB_EXTENSION)) {
		auto glbStream = streamReader->GetInputStream(pathFile.u8string()); // Pass a UTF-8 encoded filename to GetInputString
		auto glbResourceReader = std::make_unique<Microsoft::glTF::GLBResourceReader>(std::move(streamReader), std::move(glbStream));
		manifest = glbResourceReader->GetJson(); // Get the manifest from the JSON chunk
		resourceReader = std::move(glbResourceReader);
	}

	if (!resourceReader) {
		throw std::runtime_error("[ModelFactory] filename extension must be .gltf or .glb");
	}

	Microsoft::glTF::Document document;
	try {
		document = Microsoft::glTF::Deserialize(manifest);
	} catch (const Microsoft::glTF::GLTFException& ex) {
		std::stringstream ss;
		ss << "Microsoft::glTF::Deserialize failed: ";
		ss << ex.what();
		throw std::runtime_error(ss.str());
	}

	return document;
}

///////////////////////////////////////////////////////////////////////////
////////////////////////////// STREAM READER //////////////////////////////
///////////////////////////////////////////////////////////////////////////

StreamReader::StreamReader(std::filesystem::path basePath)
	: m_pathBase(std::move(basePath))
{
	assert(m_pathBase.has_root_path());
}

std::shared_ptr<std::istream> StreamReader::GetInputStream(const std::string& filename) const {
	// In order to construct a valid stream:
	// 1. The filename argument will be encoded as UTF-8 so use filesystem::u8path to
	//    correctly construct a path instance.
	// 2. Generate an absolute path by concatenating m_pathBase with the specified filename
	//    path. The filesystem::operator/ uses the platform's preferred directory separator
	//    if appropriate.
	// 3. Always open the file stream in binary mode. The glTF SDK will handle any text
	//    encoding issues for us.
	auto streamPath = m_pathBase / std::filesystem::u8path(filename);
	auto stream = std::make_shared<std::ifstream>(streamPath, std::ios_base::binary);

	// Check if the stream has no errors and is ready for I/O operations
	if (!stream || !(*stream)) {
		throw std::runtime_error("[StreamReader] Unable to create a valid input stream for uri: " + filename);
	}

	return stream;
}
