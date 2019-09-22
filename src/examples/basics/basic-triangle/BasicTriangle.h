#pragma once

#include "examples/IExample.h"
#include "core/Context.h"
#include "systems/ISystem.h"

namespace basicExample {
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
		std::vector<std::shared_ptr<ISystem>> m_systems;
	};
}
