#include "pch.h"
#include "PointLight.h"

#include "graphics/ConstantBuffer.h"
#include "factories/components/MeshPrimitiveFactory.h"
#include "systems/RenderSystem.h"
#include "systems/CameraSystem.h"
#include "components/physics/Transform.h"
#include "components/singletons/graphics/ConstantBuffers.h"
#include "components/singletons/graphics/Lights.h"

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

		// Light
		scomp::Lights& lights = m_ctx.registry.get<scomp::Lights>(graphEntity);
		lights.hasToBeUpdated = true;

		scomp::PointLight light0 = {};
		light0.position = XMFLOAT3(2, 2, -2);
		light0.color = XMFLOAT3(1, 1, 1);
		light0.intensity = 1.0f;
		light0.attenuationRadius = 10.0f;
		lights.pointLights.push_back(light0);

		// Init non-optionnal constant buffer
		comp::ConstantBuffer perLightCB = m_ctx.rcommand->CreateConstantBuffer(sizeof(cb::perLightChange) * 1);
		cbs.constantBuffers.at(scomp::ConstantBufferIndex::PER_LIGHT_CHANGE) = perLightCB;

		// Vertex shader
		comp::VertexShader VShader = m_ctx.rcommand->CreateVertexShader(primFactory.GetIed(), primFactory.GetIedElementCount(), L"res/built-shaders/BlinnPointLight_VS.cso");
		VShader.constantBuffers.push_back(cbs.constantBuffers.at(scomp::ConstantBufferIndex::PER_MESH).buffer);
		VShader.constantBuffers.push_back(cbs.constantBuffers.at(scomp::ConstantBufferIndex::PER_FRAME).buffer);

		// Pixel Shader
		comp::PixelShader PShader = m_ctx.rcommand->CreatePixelShader(L"res/built-shaders/BlinnPointLight_PS.cso");
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
		m_litEntity = entity;

		// Light representation
		transform.position = XMFLOAT3(2, 2, -2);
		transform.scale = XMFLOAT3(0.2f, 0.2f, 0.2f);

		auto entity2 = m_ctx.registry.create();
		m_ctx.registry.assign<comp::VertexShader>(entity2, VShader);
		m_ctx.registry.assign<comp::PixelShader>(entity2, PShader);
		m_ctx.registry.assign<comp::Mesh>(entity2, mesh);
		m_ctx.registry.assign<comp::Transform>(entity2, transform);
		m_lightEntity = entity2;
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

		comp::Transform& transform = m_ctx.registry.get<comp::Transform>(m_litEntity);
		ImGui::Text("Lit object :");
		ImGui::SliderFloat3("Position", (float *) &transform.position, -4, 4);
		ImGui::SliderFloat3("Scale", (float *) &transform.scale, -4, 4);
		ImGui::SliderFloat3("Rotation", (float*)& transform.rotationEuler, 0, XM_PI * 2);
		XMStoreFloat4(&transform.rotation, XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&transform.rotationEuler)));

		ImGui::End();
	}
}
