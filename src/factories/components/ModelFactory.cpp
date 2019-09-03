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
	fx::gltf::Document doc = fx::gltf::LoadFromText("res/models/cube/Cube.gltf");
	
	// TODO do it for all and not only index 0
	fx::gltf::Mesh const& mesh = doc.meshes[0];
	fx::gltf::Primitive const& primitive = mesh.primitives[0];
	
	GltfBufferInfo vertexBuffer = {};
	GltfBufferInfo normalBuffer = {};
	GltfBufferInfo tangentBuffer = {};
	GltfBufferInfo texCoord0Buffer = {};
	GltfBufferInfo indexBuffer = {};

	for (auto const& attrib : primitive.attributes) {
		if (attrib.first == "POSITION") {
			vertexBuffer = GetData(doc, doc.accessors[attrib.second]);
		} else if (attrib.first == "NORMAL") {
			normalBuffer = GetData(doc, doc.accessors[attrib.second]);
		} else if (attrib.first == "TANGENT") {
			tangentBuffer = GetData(doc, doc.accessors[attrib.second]);
		} else if (attrib.first == "TEXCOORD_0") {
			texCoord0Buffer = GetData(doc, doc.accessors[attrib.second]);
		}
	}
	indexBuffer = GetData(doc, doc.accessors[primitive.indices]);

	if (primitive.material >= 0) {
		// TODO get material data
	}

	// TODO create buffer to directX

	return comp::Model();
}

ModelFactory::GltfBufferInfo ModelFactory::GetData(fx::gltf::Document const& doc, fx::gltf::Accessor const& accessor) {
	fx::gltf::BufferView const& bufferView = doc.bufferViews[accessor.bufferView];
	fx::gltf::Buffer const& buffer = doc.buffers[bufferView.buffer];

	uint32_t dataTypeSize = CalculateDataTypeSize(accessor);
	return GltfBufferInfo{ &accessor, &buffer.data[static_cast<uint64_t>(bufferView.byteOffset) + accessor.byteOffset], dataTypeSize, accessor.count * dataTypeSize };
}

uint32_t ModelFactory::CalculateDataTypeSize(fx::gltf::Accessor const& accessor) {
	uint32_t elementSize = 0;
	switch (accessor.componentType) {
	case fx::gltf::Accessor::ComponentType::Byte:
	case fx::gltf::Accessor::ComponentType::UnsignedByte:
		elementSize = 1;
		break;
	case fx::gltf::Accessor::ComponentType::Short:
	case fx::gltf::Accessor::ComponentType::UnsignedShort:
		elementSize = 2;
		break;
	case fx::gltf::Accessor::ComponentType::Float:
	case fx::gltf::Accessor::ComponentType::UnsignedInt:
		elementSize = 4;
		break;
	}

	switch (accessor.type) {
	case fx::gltf::Accessor::Type::Mat2:
		return 4 * elementSize;
		break;
	case fx::gltf::Accessor::Type::Mat3:
		return 9 * elementSize;
		break;
	case fx::gltf::Accessor::Type::Mat4:
		return 16 * elementSize;
		break;
	case fx::gltf::Accessor::Type::Scalar:
		return elementSize;
		break;
	case fx::gltf::Accessor::Type::Vec2:
		return 2 * elementSize;
		break;
	case fx::gltf::Accessor::Type::Vec3:
		return 3 * elementSize;
		break;
	case fx::gltf::Accessor::Type::Vec4:
		return 4 * elementSize;
		break;
	}

	return 0;
}
