#include "pch.h"
#include "ModelFactory.h"

///////////////////////////////////////////////////////////////////////////
////////////////////////////// MODEL FACTORY //////////////////////////////
///////////////////////////////////////////////////////////////////////////

ModelFactory::ModelFactory(Context& context) : m_ctx(context) {
	m_ied.at(0) = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	m_ied.at(1) = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	m_ied.at(2) = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	m_ied.at(3) = { "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	m_ied.at(4) = { "BITANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
}

ModelFactory::~ModelFactory() {
}

comp::Model ModelFactory::CreateModel(const char* gltfFilePath) {
	fx::gltf::Document doc = fx::gltf::LoadFromText(gltfFilePath);
	
	// TODO do it for all and not only index 0
	fx::gltf::Mesh const& mesh = doc.meshes[0];
	fx::gltf::Primitive const& primitive = mesh.primitives[0];
	
	GltfBufferInfo positionBuffer = {};
	GltfBufferInfo normalBuffer = {};
	GltfBufferInfo tangentBuffer = {};
	GltfBufferInfo texCoord0Buffer = {};
	GltfBufferInfo indexBuffer = {};

	// https://github.com/KhronosGroup/glTF/tree/master/specification/2.0#meshes
	for (auto const& attrib : primitive.attributes) {
		if (attrib.first == "POSITION") { // FLOAT3
			positionBuffer = GetGltfBufferInfo(doc, doc.accessors[attrib.second]);
		} else if (attrib.first == "NORMAL") { // FLOAT3
			normalBuffer = GetGltfBufferInfo(doc, doc.accessors[attrib.second]);
		} else if (attrib.first == "TANGENT") { // FLOAT4
			tangentBuffer = GetGltfBufferInfo(doc, doc.accessors[attrib.second]);
		} else if (attrib.first == "TEXCOORD_0") { // FLOAT2
			texCoord0Buffer = GetGltfBufferInfo(doc, doc.accessors[attrib.second]);
		}
	}
	indexBuffer = GetGltfBufferInfo(doc, doc.accessors[primitive.indices]);

	if (primitive.material >= 0) {
		// TODO get material data
	}

	// TODO Handle occlusion and emissive maps

	// Create data to directX
	// TODO fill with vertex buffer with every other buffer (if noexistent set data to 0)
	comp::VertexBuffer vb = m_ctx.rcommand->CreateVertexBuffer((void*) positionBuffer.data, positionBuffer.vertexCount, positionBuffer.dataStride);
	comp::IndexBuffer ib = m_ctx.rcommand->CreateIndexBuffer((WORD*) indexBuffer.data, indexBuffer.vertexCount);

	comp::Model model = {};
	model.ib = ib;
	model.vb = vb;
	return model;
}

ModelFactory::GltfBufferInfo ModelFactory::GetGltfBufferInfo(fx::gltf::Document const& doc, fx::gltf::Accessor const& accessor) {
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
