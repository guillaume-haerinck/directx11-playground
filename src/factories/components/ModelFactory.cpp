#include "pch.h"
#include "ModelFactory.h"

///////////////////////////////////////////////////////////////////////////
////////////////////////////// MODEL FACTORY //////////////////////////////
///////////////////////////////////////////////////////////////////////////

ModelFactory::ModelFactory(Context& context) : m_ctx(context) {
	
}

ModelFactory::~ModelFactory() {
}

comp::Model ModelFactory::CreateModel(const char* gltfFilePath) {
	fx::gltf::Document doc = fx::gltf::LoadFromText(gltfFilePath);
	
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

	// Create data to directX
	// TODO handle when a buffer is missing
	comp::VertexBuffer vb = m_ctx.rcommand->CreateVertexBuffer((void*) vertexBuffer.data, vertexBuffer.vertexCount, vertexBuffer.dataStride);
	comp::IndexBuffer ib = m_ctx.rcommand->CreateIndexBuffer((WORD*) indexBuffer.data, indexBuffer.vertexCount);

	comp::Model model = {};
	model.ib = ib;
	model.vb = vb;
	return model;
}

ModelFactory::GltfBufferInfo ModelFactory::GetData(fx::gltf::Document const& doc, fx::gltf::Accessor const& accessor) {
	fx::gltf::BufferView const& bufferView = doc.bufferViews[accessor.bufferView];
	fx::gltf::Buffer const& buffer = doc.buffers[bufferView.buffer];

	uint32_t dataTypeSize = CalculateDataTypeSize(accessor);
	return GltfBufferInfo{ &accessor, &buffer.data[static_cast<uint64_t>(bufferView.byteOffset) + accessor.byteOffset], dataTypeSize, accessor.count * dataTypeSize, accessor.count };
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
