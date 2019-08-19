#pragma once

#include "i-exemple.h"
#include "graphics/DXObjects.h"
#include "graphics/Shader.h"

struct ConstantBuffer {
	DirectX::XMMATRIX World;
	DirectX::XMMATRIX View;
	DirectX::XMMATRIX Projection;
};

namespace exemple {
	class RotatingCube : public IExemple {
	public:
		RotatingCube(DXObjects dxObjects);
		~RotatingCube();

		virtual void Update() override;
		virtual void ImGuiUpdate() override;

	private:
		DXObjects m_dxo;
		std::unique_ptr<Shader> m_shader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
	};
}
