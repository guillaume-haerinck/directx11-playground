#pragma once

#include "examples/IExample.h"
#include "core/Context.h"
#include "graphics/RenderCommand.h"

namespace exemple {
	/**
	 * @brief The most simple exemple. Render a triangle on screen
	 */
	class BasicTriangle : public IExample {
	public:
		BasicTriangle(Context& context);
		virtual ~BasicTriangle();

		virtual void Update() override;
		virtual void ImGuiUpdate() override;

	private:
		Context& m_ctx;
	};
}
