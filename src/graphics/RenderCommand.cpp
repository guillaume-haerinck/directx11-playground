#include "pch.h"
#include "RenderCommand.h"

#include "DXException.h"

RenderCommand::RenderCommand(DXObjects& dxObjects) : m_dxo(dxObjects)
{
	
}

void RenderCommand::Clear() const {
	const float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_dxo.context->ClearRenderTargetView(m_dxo.target.Get(), color);
	m_dxo.context->ClearDepthStencilView(m_dxo.depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void RenderCommand::Swap() const {
	m_dxo.swapChain->Present(1u, 0u);
}
