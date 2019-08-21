#pragma once

#include "IExemple.h"
#include "graphics/DXObjects.h"
#include "graphics/Shader.h"
#include "graphics/Buffer.h"

namespace exemple {
	/**
	 * @brief The most simple exemple. Render a triangle on screen
	 */
	class BasicTriangle : public IExemple {
	public:
		BasicTriangle(DXObjects dxObjects);
		~BasicTriangle();

		virtual void Update() override;
		virtual void ImGuiUpdate() override;

	private:
		DXObjects m_dxo;
		std::unique_ptr<Shader> m_shader;
		std::unique_ptr<VertexBuffer> m_vertexBuffer;
	};
}
