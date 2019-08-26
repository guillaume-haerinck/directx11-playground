#pragma once

#include "exemples/IExemple.h"
#include "graphics/DXObjects.h"
#include "graphics/Shader.h"
#include "graphics/Buffer.h"
#include "factories/ModelFactory.h"

namespace exemple {
	class ModelLoading : public IExemple {
	public:
		ModelLoading(DXObjects& m_dxo);
		virtual ~ModelLoading();

		virtual void Update() override;
		virtual void ImGuiUpdate() override;

	private:
		DXObjects& m_dxo;
		std::unique_ptr<Shader> m_shader;
		std::unique_ptr<IndexBuffer> m_indexBuffer;
		std::unique_ptr<VertexBuffer> m_vertexBuffer;
		ModelFactory m_modelFactory;
	};
}
