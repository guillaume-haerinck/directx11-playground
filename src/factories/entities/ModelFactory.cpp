#include "pch.h"
#include "ModelFactory.h"

#include "components/singletons/graphics/Materials.h"
#include "components/graphics/Mesh.h"

///////////////////////////////////////////////////////////////////////////
////////////////////////////// MODEL FACTORY //////////////////////////////
///////////////////////////////////////////////////////////////////////////

ModelFactory::ModelFactory(Context& context) : m_ctx(context) {
	m_ied.at(0) = { "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	m_ied.at(1) = { "NORMAL",	 0, DXGI_FORMAT_R32G32B32_FLOAT,	1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	m_ied.at(2) = { "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,		2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	// m_ied.at(3) = { "TANGENT",	 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 3, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	// m_ied.at(4) = { "BITANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 4, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
}

ModelFactory::~ModelFactory() {
}

std::vector<entt::entity> ModelFactory::CreateEntitiesFromGltf(std::filesystem::path gltfFilePath) {
	fx::gltf::Document doc = fx::gltf::LoadFromText(gltfFilePath.string());
	std::vector<entt::entity> entities;
	// TODO handle scene transform and nodes
	// TODO create default material

	for (auto const& meshInfo : doc.meshes) {
		for (auto const& primitive : meshInfo.primitives) {
			GltfBufferInfo positionBufferInfo = {};
			GltfBufferInfo normalBufferInfo = {};
			GltfBufferInfo texCoord0BufferInfo = {};
			GltfBufferInfo tangentBufferInfo = {};

			// Get attribute buffers
			// https://github.com/KhronosGroup/glTF/tree/master/specification/2.0#meshes
			for (auto const& attrib : primitive.attributes) {
				if (attrib.first == "POSITION") { // FLOAT3
					positionBufferInfo = GetGltfBufferInfo(doc, doc.accessors[attrib.second]);
				} else if (attrib.first == "NORMAL") { // FLOAT3
					normalBufferInfo = GetGltfBufferInfo(doc, doc.accessors[attrib.second]);
				} else if (attrib.first == "TEXCOORD_0") { // FLOAT2
					texCoord0BufferInfo = GetGltfBufferInfo(doc, doc.accessors[attrib.second]);
				} else if (attrib.first == "TANGENT") { // FLOAT4
					tangentBufferInfo = GetGltfBufferInfo(doc, doc.accessors[attrib.second]);
				} 
			}

			// Create attribute buffers
			// TODO handle when one buffer is missing
			comp::AttributeBuffer positionBuffer = m_ctx.rcommand->CreateAttributeBuffer((void*) positionBufferInfo.data,	positionBufferInfo.vertexCount,		positionBufferInfo.dataStride);
			comp::AttributeBuffer normalBuffer = m_ctx.rcommand->CreateAttributeBuffer((void*) normalBufferInfo.data,		normalBufferInfo.vertexCount,		normalBufferInfo.dataStride);
			comp::AttributeBuffer texCoordBuffer = m_ctx.rcommand->CreateAttributeBuffer((void*) texCoord0BufferInfo.data,	texCoord0BufferInfo.vertexCount,	texCoord0BufferInfo.dataStride);
			// comp::AttributeBuffer tangentBuffer = m_ctx.rcommand->CreateAttributeBuffer((void*) tangentBufferInfo.data,		tangentBufferInfo.vertexCount,		tangentBufferInfo.dataStride);

			// Create vertex buffer
			comp::VertexBuffer vb = {};
			vb.buffers =	{ positionBuffer.buffer,	normalBuffer.buffer,	texCoordBuffer.buffer,		};
			vb.byteWidths = { positionBuffer.byteWidth,	normalBuffer.byteWidth, texCoordBuffer.byteWidth,	};
			vb.counts =		{ positionBuffer.count,		normalBuffer.count,		texCoordBuffer.count,		};
			vb.strides =	{ positionBuffer.stride,	normalBuffer.stride,	texCoordBuffer.stride,		};
			vb.offsets =	{ 0,						0,						0,							};
			vb.names =		{ "position",				"normal",				"texture coordinates",		};

			// Get and create index buffer
			GltfBufferInfo indexBufferInfo = {};
			indexBufferInfo = GetGltfBufferInfo(doc, doc.accessors[primitive.indices]);
			comp::IndexBuffer ib = m_ctx.rcommand->CreateIndexBuffer((void*) indexBufferInfo.data, indexBufferInfo.vertexCount);

			// Add the mesh to the model
			comp::Mesh mesh = {};
			mesh.vb = vb;
			mesh.ib = ib;

			// TODO use child component for transforms

			// Get material
			// TODO use a hashmap to check if material already created, if it is use the index
			if (primitive.material >= 0) {
				fx::gltf::Material material = doc.materials[primitive.material];

				// Base color texture
				int32_t baseColorTexIndex = material.pbrMetallicRoughness.baseColorTexture.index;
				std::string textureName = doc.images.at(baseColorTexIndex).uri;
				std::filesystem::path texturePath = gltfFilePath.parent_path().append(textureName);
				scomp::Texture texture = m_ctx.rcommand->CreateTexture(0, texturePath.wstring().c_str());
				mesh.textures.push_back(texture.srv);

			} else {
				mesh.materialIndex = 0;
			}

			// Create new entity
			auto entity = m_ctx.registry.create();
			entities.push_back(entity);
			m_ctx.registry.assign<comp::Mesh>(entity, mesh);
		}
	}

	return entities;
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
