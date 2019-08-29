#pragma once

#include "exemples/IExemple.h"
#include "core/StepTimer.h"
#include "core/Context.h"
#include "factories/PrimitiveFactory.h"

namespace exemple {
	class RotatingCube : public IExemple {
	public:
		RotatingCube(Context& context);
		virtual ~RotatingCube();

		virtual void Update() override;
		virtual void ImGuiUpdate() override;

	private:
		DX::StepTimer m_timer;
		Context& m_ctx;
	};
}
