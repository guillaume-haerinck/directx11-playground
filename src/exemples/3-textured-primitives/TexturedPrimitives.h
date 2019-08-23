#pragma once

#include "exemples/IExemple.h"
#include "core/StepTimer.h"
#include "graphics/DXObjects.h"
#include "graphics/Shader.h"
#include "graphics/Buffer.h"
#include "factories/PrimitiveFactory.h"

namespace exemple {
	class TexturedPrimitives : public IExemple {
	public:
		TexturedPrimitives(DXObjects& dxObjects);
		~TexturedPrimitives();

		virtual void Update() override;
		virtual void ImGuiUpdate() override;

	private:
		DX::StepTimer m_timer;
		DXObjects& m_dxo;
		std::unique_ptr<Shader> m_shader;
		std::unique_ptr<IndexBuffer> m_indexBuffer;
		std::unique_ptr<VertexBuffer> m_vertexBuffer;

		Sphere m_sphere;
	};
};

