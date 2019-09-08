#include "pch.h"
#include "TexturedPrimitives.h"

#include "graphics/DXException.h"
#include "factories/components/MeshPrimitiveFactory.h"
#include "systems/RenderSystem.h"
#include "components/singletons/graphics/Material.h"
#include "graphics/ConstantBuffers.h"
#include "components/singletons/graphics/Camera.h"

namespace exemple {
	TexturedPrimitives::TexturedPrimitives(Context& context) : m_ctx(context) {
		// Init
		MeshPrimitiveFactory primFactory(context);
		m_systems.push_back(std::make_unique<RenderSystem>(context));
		auto entity = m_ctx.registry.create();

		// Shader
		comp::VertexShader VShader = m_ctx.rcommand->CreateVertexShader(primFactory.GetIed(), primFactory.GetIedElementCount(), L"res/built-shaders/TexturedPrimitives_VS.cso");
		comp::PixelShader PShader = m_ctx.rcommand->CreatePixelShader(L"res/built-shaders/TexturedPrimitives_PS.cso");
		comp::ConstantBuffer VSCB0 = m_ctx.rcommand->CreateConstantBuffer(0, (sizeof(cb::TEMP)));
		VShader.constantBuffers.push_back(VSCB0);
		m_ctx.registry.assign<comp::VertexShader>(entity, VShader);
		m_ctx.registry.assign<comp::PixelShader>(entity, PShader);

		// TODO find a more global and safer way to init singleton components (use POO instead ?)
		// Init camera constant buffer
		auto graphEntity = m_ctx.singletonComponents.at(SingletonComponents::GRAPHIC);
		scomp::Camera camera = {};
		camera.constantBuffer = VSCB0;
		m_ctx.registry.assign<scomp::Camera>(graphEntity, camera);

		// Material
		scomp::PhongMaterial material = {};
		auto texture = m_ctx.rcommand->CreateTexture(scomp::PhongTexSlot::DIFFUSE, L"res/textures/test.jpg");
		material.textures.push_back(texture);
		m_ctx.registry.assign<scomp::PhongMaterial>(entity, material);
		// TODO assign material to singleton entity and use index in component

		// Mesh
		auto mesh = primFactory.CreateIcosahedron();
		m_ctx.registry.assign<comp::Mesh>(entity, mesh);
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
