#pragma once

#include "i-exemple.h"
#include "graphics/DXObjects.h"
#include "graphics/Shader.h"

namespace exemple {
	class BasicTriangle : public IExemple {
	public:
		BasicTriangle(DXObjects dxObjects);
		~BasicTriangle();

		virtual void Update() override;
		virtual void ImGuiUpdate() override;

	private:
		DXObjects m_dxo;
		std::unique_ptr<Shader> m_shader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	};
}
