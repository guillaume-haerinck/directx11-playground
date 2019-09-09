#pragma once

#include "examples/IExample.h"
#include "core/Context.h"

namespace phongExample {
	class PointLight : public IExample {
	public:
		PointLight(Context& context);
		virtual ~PointLight();

		void Update() override;
		void ImGuiUpdate() override;

	private:
		Context& m_ctx;
	};
}
