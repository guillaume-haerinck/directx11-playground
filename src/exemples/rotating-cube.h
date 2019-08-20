#pragma once

#include "i-exemple.h"
#include "graphics/DXObjects.h"
#include "graphics/Shader.h"

struct ConstantBuffer {
	DirectX::XMFLOAT4X4 matVP;
	DirectX::XMFLOAT4X4 matGeo;
};

struct Vertex {
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT4 Color;
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
