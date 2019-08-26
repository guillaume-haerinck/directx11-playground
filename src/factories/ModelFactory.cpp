#include "pch.h"
#include "ModelFactory.h"

///////////////////////////////////////////////////////////////////////////
////////////////////////////// MODEL FACTORY //////////////////////////////
///////////////////////////////////////////////////////////////////////////

ModelFactory::ModelFactory(std::filesystem::path gltfFilePath) {
	// Convert the relative path into an absolute path
	auto currentPath = std::filesystem::current_path();
	currentPath /= gltfFilePath;
	currentPath.swap(gltfFilePath);

	auto streamReader = std::make_unique<StreamReader>(gltfFilePath.parent_path());
	std::unique_ptr<Microsoft::glTF::GLTFResourceReader> resourceReader;

	std::filesystem::path filename = gltfFilePath.filename();
	auto gltfStream = streamReader->GetInputStream(filename.u8string());
	auto gltfResourceReader = std::make_unique<Microsoft::glTF::GLTFResourceReader>(std::move(streamReader));

	// Read the contents of the glTF file into a string using a std::stringstream
	std::stringstream manifestStream;
	manifestStream << gltfStream->rdbuf();
	std::string manifest = manifestStream.str();

	resourceReader = std::move(gltfResourceReader);

	Microsoft::glTF::Document document;
	try {
		document = Microsoft::glTF::Deserialize(manifest);
	} catch (const Microsoft::glTF::GLTFException& ex) {
		std::stringstream ss;
		ss << "Microsoft::glTF::Deserialize failed: ";
		ss << ex.what();
		throw std::runtime_error(ss.str());
	}

	document.meshes;
}

ModelFactory::~ModelFactory()
{
}

///////////////////////////////////////////////////////////////////////////
////////////////////////////// STREAM READER //////////////////////////////
///////////////////////////////////////////////////////////////////////////

StreamReader::StreamReader(std::filesystem::path basePath)
	: m_pathBase(std::move(basePath))
{
	assert(m_pathBase.has_root_path());
}

StreamReader::~StreamReader()
{
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
