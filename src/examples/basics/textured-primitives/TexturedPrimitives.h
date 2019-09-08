#pragma once

#include "examples/IExample.h"
#include "core/Context.h"
#include "systems/ISystem.h"

namespace exemple {
	class TexturedPrimitives : public IExample {
	public:
		TexturedPrimitives(Context& context);
		virtual ~TexturedPrimitives();

		virtual void Update() override;
		virtual void ImGuiUpdate() override;

	private:
		Context& m_ctx;
		std::vector<std::unique_ptr<ISystem>> m_systems;
	};
};

