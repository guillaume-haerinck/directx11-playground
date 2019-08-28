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
		std::unique_ptr<RenderCommand> m_rcommand;
		entt::registry registry;
	};
}
