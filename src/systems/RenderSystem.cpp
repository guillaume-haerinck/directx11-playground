#include "pch.h"
#include "RenderSystem.h"

RenderSystem::RenderSystem(Context& context) : m_ctx(context) {
}

RenderSystem::~RenderSystem() {
}

void RenderSystem::Update() {
	// Draw geometry
	// TODO see how to handle instanced draw
	m_ctx.registry.view<comp::Mesh, comp::VertexShader, comp::PixelShader>()
		.each([&](comp::Mesh& mesh, comp::VertexShader& VShader, comp::PixelShader& PShader) {
		m_ctx.rcommand->BindVertexShader(VShader);
		m_ctx.rcommand->BindPixelShader(PShader);
		m_ctx.rcommand->BindVertexBuffer(mesh.vb);

		if (mesh.ib.count > 0) {
			m_ctx.rcommand->BindIndexBuffer(mesh.ib);
			m_ctx.rcommand->DrawIndexed(mesh.ib.count);
		} else {
			m_ctx.rcommand->Draw(mesh.vb.count);
		}
	});
}
