#pragma once

#include "IExemple.h"
#include "core/StepTimer.h"
#include "graphics/DXObjects.h"
#include "graphics/Shader.h"
#include "graphics/Buffer.h"

struct ConstantBufferRect {
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
		std::unique_ptr<IndexBuffer> m_indexBuffer;
		std::unique_ptr<VertexBuffer> m_vertexBuffer;
	};
}
