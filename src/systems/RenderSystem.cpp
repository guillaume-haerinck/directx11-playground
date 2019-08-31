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

		for (auto texture : mesh.textures) {
			// TODO handle texture slots
			// TODO handle layering to draw the objects with the same textures at the same time
			m_ctx.rcommand->BindTexture(texture);
		}

		if (mesh.ib.count > 0) {
			m_ctx.rcommand->BindIndexBuffer(mesh.ib);
			m_ctx.rcommand->DrawIndexed(mesh.ib.count);
		} else {
			m_ctx.rcommand->Draw(mesh.vb.count);
		}
	});
}
