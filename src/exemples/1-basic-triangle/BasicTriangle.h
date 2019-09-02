#pragma once

#include "exemples/IExemple.h"
#include "core/Context.h"
#include "graphics/RenderCommand.h"

namespace exemple {
	/**
	 * @brief The most simple exemple. Render a triangle on screen
	 */
	class BasicTriangle : public IExemple {
	public:
		BasicTriangle(Context& context);
		virtual ~BasicTriangle();

		virtual void Update() override;
		virtual void ImGuiUpdate() override;

	private:
		Context& m_ctx;
	};
}
