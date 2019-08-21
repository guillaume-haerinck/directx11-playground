#pragma once

#include "i-exemple.h"
#include "core/StepTimer.h"
#include "graphics/DXObjects.h"
#include "graphics/Shader.h"

struct ConstantBuffer {
	XMFLOAT4X4 matVP;
	XMFLOAT4X4 matGeo;
};

struct Vertex {
	XMFLOAT3 Position;
	XMFLOAT4 Color;
};

namespace exemple {
	class RotatingCube : public IExemple {
	public:
		RotatingCube(DXObjects dxObjects);
		~RotatingCube();

		virtual void Update() override;
		virtual void ImGuiUpdate() override;

	private:
		DX::StepTimer m_timer;
		DXObjects m_dxo;
		std::unique_ptr<Shader> m_shader;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
	};
}
