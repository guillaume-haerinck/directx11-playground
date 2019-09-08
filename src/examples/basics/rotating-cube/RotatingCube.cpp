#include "pch.h"
#include "RotatingCube.h"

#include "factories/components/MeshPrimitiveFactory.h"
#include "components/graphics/Mesh.h"
#include "systems/RenderSystem.h"
#include "graphics/ConstantBuffers.h"
#include "components/singletons/graphics/Camera.h"

namespace exemple {
	RotatingCube::RotatingCube(Context& context) : m_ctx(context) {
		// Init
		MeshPrimitiveFactory primFactory(context);
		m_systems.push_back(std::make_unique<RenderSystem>(context));

		// Vertex Shader
		comp::VertexShader VShader = m_ctx.rcommand->CreateVertexShader(primFactory.GetIed(), primFactory.GetIedElementCount(), L"res/built-shaders/RotatingCube_VS.cso");
		comp::ConstantBuffer VSCB0 = m_ctx.rcommand->CreateConstantBuffer(0, (sizeof(cb::TEMP)));
		VShader.constantBuffers.push_back(VSCB0);

		// Pixel shader
		comp::PixelShader PShader = m_ctx.rcommand->CreatePixelShader(L"res/built-shaders/RotatingCube_PS.cso");
		comp::ConstantBuffer PSCB0 = m_ctx.rcommand->CreateConstantBuffer(0, (sizeof(XMFLOAT4) * 6));
		PShader.constantBuffers.push_back(PSCB0);

		// Update PSconstant buffer once as it will not change
		XMFLOAT4 PSCB0data[6] = {
			XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f),
			XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
			XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
			XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
			XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f),
			XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f)
		};
		m_ctx.rcommand->UpdateConstantBuffer(PSCB0, &PSCB0data);

		// TODO find a more global and safer way to init singleton components (use POO instead ?)
		// Init camera constant buffer
		auto graphEntity = m_ctx.singletonComponents.at(SingletonComponents::GRAPHIC);
		scomp::Camera camera = {};
		camera.constantBuffer = VSCB0;
		m_ctx.registry.assign<scomp::Camera>(graphEntity, camera);

		// Save data to entity
		auto entity = m_ctx.registry.create();
		comp::Mesh mesh = primFactory.CreateBox();
		m_ctx.registry.assign<comp::VertexShader>(entity, VShader);
		m_ctx.registry.assign<comp::PixelShader>(entity, PShader);
		m_ctx.registry.assign<comp::Mesh>(entity, mesh);
	}

	RotatingCube::~RotatingCube() {
	}

	void RotatingCube::Update() {
		for (auto& system : m_systems) {
			system->Update();
		}
	}

	void RotatingCube::ImGuiUpdate() {
	}
}
