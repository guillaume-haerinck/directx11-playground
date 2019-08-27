#pragma once

#include "exemples/IExemple.h"
#include "graphics/DXObjects.h"
#include "graphics/RenderCommand.h"

namespace exemple {
	/**
	 * @brief The most simple exemple. Render a triangle on screen
	 */
	class BasicTriangle : public IExemple {
	public:
		BasicTriangle(DXObjects& dxObjects);
		virtual ~BasicTriangle();

		virtual void Update() override;
		virtual void ImGuiUpdate() override;

	private:
		DXObjects& m_dxo;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VSShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PSShader;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
		std::unique_ptr<RenderCommand> m_rcommand;
	};
}
