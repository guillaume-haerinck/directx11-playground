#include "pch.h"
#include "RenderSystem.h"

#include "components/singletons/graphics/Camera.h"
#include "graphics/ConstantBuffers.h"

RenderSystem::RenderSystem(Context& context) : m_ctx(context) {
}

RenderSystem::~RenderSystem() {
}

void RenderSystem::Update() {
	m_timer.Tick([](){});

	// TODO Get camera from singleton component and update its constant buffer at each frame
	// TODO Get lights from singleton component and update its constant buffer when there is a change
	// TODO Get materials from component and update its constant buffer when there is a change

	// Update camera constant buffer
	{
		auto graphEntity = m_ctx.singletonComponents.at(SingletonComponents::GRAPHIC);
		scomp::Camera& camera = m_ctx.registry.get<scomp::Camera>(graphEntity);

		XMMATRIX view = XMMatrixTranspose(
			XMMatrixRotationZ(m_timer.GetFrameCount() * 0.01) *
			XMMatrixRotationX(m_timer.GetFrameCount() * 0.01) *
			XMMatrixTranslation(0.0f, 0.0f, 6.0f) *
			XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 10.0f)
		);

		// Update VSconstant buffer
		cb::TEMP VSCB0data;
		XMStoreFloat4x4(&VSCB0data.matVP, view);
		XMStoreFloat4x4(&VSCB0data.matGeo, XMMatrixIdentity());
		m_ctx.rcommand->UpdateConstantBuffer(camera.constantBuffer, &VSCB0data);
	}

	// Render
	m_ctx.registry.view<comp::Mesh, comp::VertexShader, comp::PixelShader>()
		.each([&](comp::Mesh& mesh, comp::VertexShader& VShader, comp::PixelShader& PShader) {
		m_ctx.rcommand->BindVertexShader(VShader);
		m_ctx.rcommand->BindPixelShader(PShader);
		m_ctx.rcommand->BindVertexBuffer(mesh.vb);
		m_ctx.rcommand->BindIndexBuffer(mesh.ib);
		m_ctx.rcommand->DrawIndexed(mesh.ib.count);
	});
}
