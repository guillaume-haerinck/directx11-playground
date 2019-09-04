#pragma once

#include "exemples/IExemple.h"
#include "core/StepTimer.h"
#include "core/Context.h"

namespace exemple {
	class TexturedPrimitives : public IExemple {
	public:
		TexturedPrimitives(Context& context);
		virtual ~TexturedPrimitives();

		virtual void Update() override;
		virtual void ImGuiUpdate() override;

	private:
		DX::StepTimer m_timer;
		Context& m_ctx;
		comp::ConstantBuffer m_VSCB0;
	};
};

