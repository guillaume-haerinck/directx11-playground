#include "pch.h"
#include "TexturedPrimitives.h"

#include "graphics/DXException.h"
#include "factories/components/MeshPrimitiveFactory.h"
#include "systems/RenderSystem.h"
#include "graphics/ConstantBuffer.h"
#include "components/singletons/graphics/ConstantBuffers.h"

namespace exemple {
	TexturedPrimitives::TexturedPrimitives(Context& context) : m_ctx(context) {
		// Init
		MeshPrimitiveFactory primFactory(context);
		m_systems.push_back(std::make_unique<RenderSystem>(context));

		// Vertex shader
		comp::VertexShader VShader = m_ctx.rcommand->CreateVertexShader(primFactory.GetIed(), primFactory.GetIedElementCount(), L"res/built-shaders/TexturedPrimitives_VS.cso");
		comp::ConstantBuffer cameraCB = m_ctx.rcommand->CreateConstantBuffer(sizeof(cb::Camera));
		comp::ConstantBuffer meshVarCB = m_ctx.rcommand->CreateConstantBuffer(sizeof(cb::MeshVariable) * 1);
		VShader.constantBuffers.push_back(cameraCB.buffer);
		VShader.constantBuffers.push_back(meshVarCB.buffer);

		// Pixel Shader
		comp::PixelShader PShader = m_ctx.rcommand->CreatePixelShader(L"res/built-shaders/TexturedPrimitives_PS.cso");
		// comp::ConstantBuffer materialCB = m_ctx.rcommand->CreateConstantBuffer(sizeof(cb::PhongMaterial) * 1);
		// PShader.constantBuffers.push_back(materialCB.buffer);

		// Set constant buffers to be updated in render system
		auto graphEntity = m_ctx.singletonComponents.at(SingletonComponents::GRAPHIC);
		scomp::ConstantBuffers& constantBuffers = m_ctx.registry.get<scomp::ConstantBuffers>(graphEntity);
		constantBuffers.constantBuffers.at(scomp::ConstantBufferIndex::CAMERA) = cameraCB;
		constantBuffers.constantBuffers.at(scomp::ConstantBufferIndex::MESH_VARIABLES) = meshVarCB;
		
		// Add default material
		scomp::Texture texture = m_ctx.rcommand->CreateTexture(0, L"res/textures/test.jpg");
		scomp::PhongMaterial material = {};
		material.textures.push_back(texture);
		scomp::PhongMaterials& materials = m_ctx.registry.get<scomp::PhongMaterials>(graphEntity);
		materials.materials.push_back(material);

		// Mesh
		comp::Mesh mesh = primFactory.CreateIcosahedron();
		mesh.materialIndex = 0;
		mesh.textures.push_back(texture.srv);

		// Assign data to an entity
		auto entity = m_ctx.registry.create();
		m_ctx.registry.assign<comp::VertexShader>(entity, VShader);
		m_ctx.registry.assign<comp::PixelShader>(entity, PShader);
		m_ctx.registry.assign<comp::Mesh>(entity, mesh);
		// TODO assign transform
	}

	TexturedPrimitives::~TexturedPrimitives() {
	}

	void TexturedPrimitives::Update() {
		for (auto& system : m_systems) {
			system->Update();
		}
	}

	void TexturedPrimitives::ImGuiUpdate() {
	}
}
