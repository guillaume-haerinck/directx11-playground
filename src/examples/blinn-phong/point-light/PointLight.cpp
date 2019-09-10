#include "pch.h"
#include "PointLight.h"

#include "graphics/DXException.h"
#include "graphics/ConstantBuffer.h"
#include "factories/components/MeshPrimitiveFactory.h"
#include "systems/RenderSystem.h"
#include "systems/CameraSystem.h"
#include "components/physics/Transform.h"
#include "components/singletons/graphics/ConstantBuffers.h"

namespace phongExample {
	PointLight::PointLight(Context& context) : m_ctx(context) {
		// Init
		MeshPrimitiveFactory primFactory(context);
		m_systems = {
			std::make_shared<CameraSystem>(context),
			std::make_shared<RenderSystem>(context)
		};

		// Get constant buffers
		auto graphEntity = m_ctx.singletonComponents.at(SingletonComponents::GRAPHIC);
		scomp::ConstantBuffers& cbs = m_ctx.registry.get<scomp::ConstantBuffers>(graphEntity);

		// Vertex shader
		comp::VertexShader VShader = m_ctx.rcommand->CreateVertexShader(primFactory.GetIed(), primFactory.GetIedElementCount(), L"res/built-shaders/BlinnPointLight_VS.cso");
		VShader.constantBuffers.push_back(cbs.constantBuffers.at(scomp::ConstantBufferIndex::PER_MESH).buffer);
		VShader.constantBuffers.push_back(cbs.constantBuffers.at(scomp::ConstantBufferIndex::PER_FRAME).buffer);

		// Pixel Shader
		comp::PixelShader PShader = m_ctx.rcommand->CreatePixelShader(L"res/built-shaders/BlinnPointLight_PS.cso");

		// Mesh
		comp::Mesh mesh = primFactory.CreateBox();

		// Transformm
		comp::Transform transform = {};

		// Assign data to an entity
		auto entity = m_ctx.registry.create();
		m_ctx.registry.assign<comp::VertexShader>(entity, VShader);
		m_ctx.registry.assign<comp::PixelShader>(entity, PShader);
		m_ctx.registry.assign<comp::Mesh>(entity, mesh);
		m_ctx.registry.assign<comp::Transform>(entity, transform);
	}

	PointLight::~PointLight()
	{
	}

	void PointLight::Update() {
		for (auto& system : m_systems) {
			system->Update();
		}
	}

	void PointLight::ImGuiUpdate() {
		ImGui::Begin("Exemple properties");
		ImGui::Text("Point light !");
		ImGui::End();
	}
}
