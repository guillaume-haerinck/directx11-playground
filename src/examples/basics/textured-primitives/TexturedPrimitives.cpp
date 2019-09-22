#include "pch.h"
#include "TexturedPrimitives.h"

#include "graphics/DXException.h"
#include "factories/components/MeshPrimitiveFactory.h"
#include "factories/scomponents/ShaderFactory.h"
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
		ShaderFactory shaderFactory(context);
		m_systems = {
			std::make_shared<CameraSystem>(context),
			std::make_shared<RenderSystem>(context)
		};

		// Vertex shader
		shaderFactory.SetIed(primFactory.GetIed(), primFactory.GetIedElementCount());
		scomp::ConstantBufferIndex vsCbArray[] = {
			scomp::ConstantBufferIndex::PER_MESH,
			scomp::ConstantBufferIndex::PER_FRAME
		};
		unsigned int vsID = shaderFactory.CreateVertexShader(L"res/built-shaders/TexturedPrimitives_VS.cso", vsCbArray, ARRAYSIZE(vsCbArray));

		// Pixel Shader
		unsigned int psID = shaderFactory.CreatePixelShader(L"res/built-shaders/TexturedPrimitives_PS.cso");
		
		// Material
		auto graphEntity = m_ctx.singletonComponents.at(scomp::SingletonEntities::SING_ENTITY_GRAPHIC);
		scomp::PhongMaterials& materials = m_ctx.registry.get<scomp::PhongMaterials>(graphEntity);
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
		pipeline.vsIndex = vsID;
		pipeline.psIndex = psID;

		// Transform
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
