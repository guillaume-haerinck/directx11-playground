#include "pch.h"
#include "TexturedPrimitives.h"

#include "graphics/DXException.h"
#include "factories/components/MeshPrimitiveFactory.h"
#include "systems/RenderSystem.h"
#include "systems/CameraSystem.h"
#include "graphics/ConstantBuffer.h"
#include "components/physics/Transform.h"
#include "components/graphics/Pipeline.h"
#include "scomponents/graphics/ConstantBuffers.h"
#include "scomponents/graphics/Shaders.h"

namespace basicExample {
	TexturedPrimitives::TexturedPrimitives(Context& context) : m_ctx(context) {
		// Init
		MeshPrimitiveFactory primFactory(context);
		m_systems = {
			std::make_shared<CameraSystem>(context),
			std::make_shared<RenderSystem>(context)
		};

		// Get singleton components
		auto graphEntity = m_ctx.singletonComponents.at(scomp::SingletonEntities::SING_ENTITY_GRAPHIC);
		scomp::ConstantBuffers& cbs = m_ctx.registry.get<scomp::ConstantBuffers>(graphEntity);
		scomp::Shaders& shaders = m_ctx.registry.get<scomp::Shaders>(graphEntity);
		scomp::PhongMaterials& materials = m_ctx.registry.get<scomp::PhongMaterials>(graphEntity);

		// Vertex shader
		scomp::VertexShader VShader = m_ctx.rcommand->CreateVertexShader(primFactory.GetIed(), primFactory.GetIedElementCount(), L"res/built-shaders/TexturedPrimitives_VS.cso");
		VShader.constantBuffers.push_back(cbs.constantBuffers.at(scomp::ConstantBufferIndex::PER_MESH).buffer);
		VShader.constantBuffers.push_back(cbs.constantBuffers.at(scomp::ConstantBufferIndex::PER_FRAME).buffer);
		shaders.vss.push_back(VShader);

		// Pixel Shader
		scomp::PixelShader PShader = m_ctx.rcommand->CreatePixelShader(L"res/built-shaders/TexturedPrimitives_PS.cso");
		shaders.pss.push_back(PShader);
		
		// Material
		scomp::PhongMaterial material = {};
		scomp::Texture texture = m_ctx.rcommand->CreateTexture(0, L"res/textures/test.jpg");
		material.textures.push_back(texture);
		materials.materials.push_back(material);

		// Mesh
		comp::Mesh mesh = primFactory.CreateIcosahedron();
		mesh.materialIndex = materials.materials.size() - 1;
		mesh.materialType = scomp::MaterialType::PHONG;

		// Pipeline
		comp::Pipeline pipeline = {};
		pipeline.hasShader.at(comp::PipelineShaderIndex::PS) = true;
		pipeline.hasShader.at(comp::PipelineShaderIndex::VS) = true;
		pipeline.vsIndex = shaders.vss.size() - 1;
		pipeline.psIndex = shaders.pss.size() - 1;

		// Transformm
		comp::Transform transform = {};

		// Assign data to an entity
		auto entity = m_ctx.registry.create();
		m_ctx.registry.assign<comp::Pipeline>(entity, pipeline);
		m_ctx.registry.assign<comp::Mesh>(entity, mesh);
		m_ctx.registry.assign<comp::Transform>(entity, transform);
	}

	TexturedPrimitives::~TexturedPrimitives() {
	}

	void TexturedPrimitives::Update() {
		for (auto& system : m_systems) {
			system->Update();
		}
	}

	void TexturedPrimitives::ImGuiUpdate() {
		ImGui::Begin("Exemple properties");
		ImGui::Text("Hello texture !");
		ImGui::End();
	}
}
