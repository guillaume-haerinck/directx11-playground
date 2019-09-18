#include "pch.h"
#include "PhongMultipleLightTypes.h"

#include "graphics/ConstantBuffer.h"
#include "factories/components/MeshPrimitiveFactory.h"
#include "systems/RenderSystem.h"
#include "systems/CameraSystem.h"
#include "components/physics/Transform.h"
#include "scomponents/graphics/ConstantBuffers.h"
#include "scomponents/graphics/Lights.h"

namespace phongExample {
	PhongMultipleLightTypes::PhongMultipleLightTypes(Context& context) : m_ctx(context) {
		// Init
		MeshPrimitiveFactory primFactory(context);
		m_systems = {
			std::make_shared<CameraSystem>(context),
			std::make_shared<RenderSystem>(context)
		};

		// Get constant buffers
		auto graphEntity = m_ctx.singletonComponents.at(SingletonComponents::GRAPHIC);
		scomp::ConstantBuffers& cbs = m_ctx.registry.get<scomp::ConstantBuffers>(graphEntity);

		// Light
		scomp::Lights& lights = m_ctx.registry.get<scomp::Lights>(graphEntity);
		lights.hasToBeUpdated = true;

		scomp::SpotLight slight0 = {};
		slight0.position = DX::XMFLOAT3(0, 1, 0);
		lights.spotLights.push_back(slight0);

		scomp::PointLight plight0 = {};
		plight0.position = DX::XMFLOAT3(1, 0, 0);
		lights.pointLights.push_back(plight0);
		
		// Init non-optionnal constant buffer
		comp::ConstantBuffer perLightCB = m_ctx.rcommand->CreateConstantBuffer(sizeof(cb::perLightChange) * 2);
		cbs.constantBuffers.at(scomp::ConstantBufferIndex::PER_LIGHT_CHANGE) = perLightCB;

		// Vertex shader
		comp::VertexShader VShader = m_ctx.rcommand->CreateVertexShader(primFactory.GetIed(), primFactory.GetIedElementCount(), L"res/built-shaders/PhongMultipleLightTypes_VS.cso");
		VShader.constantBuffers.push_back(cbs.constantBuffers.at(scomp::ConstantBufferIndex::PER_MESH).buffer);
		VShader.constantBuffers.push_back(cbs.constantBuffers.at(scomp::ConstantBufferIndex::PER_FRAME).buffer);

		// Pixel Shader
		comp::PixelShader PShader = m_ctx.rcommand->CreatePixelShader(L"res/built-shaders/PhongMultipleLightTypes_PS.cso");
		PShader.constantBuffers.push_back(cbs.constantBuffers.at(scomp::ConstantBufferIndex::PER_LIGHT_CHANGE).buffer);

		// Mesh
		comp::Mesh mesh = primFactory.CreateBox();

		// Transform
		comp::Transform transform = {};

		// Assign data to an entity
		auto entity = m_ctx.registry.create();
		m_ctx.registry.assign<comp::VertexShader>(entity, VShader);
		m_ctx.registry.assign<comp::PixelShader>(entity, PShader);
		m_ctx.registry.assign<comp::Mesh>(entity, mesh);
		m_ctx.registry.assign<comp::Transform>(entity, transform);
	}

	PhongMultipleLightTypes::~PhongMultipleLightTypes() {
	}

	void PhongMultipleLightTypes::Update() {
		for (auto& system : m_systems) {
			system->Update();
		}
	}

	void PhongMultipleLightTypes::ImGuiUpdate() {
	}
}
