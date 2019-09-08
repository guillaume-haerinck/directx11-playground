#pragma once

#include "ISystem.h"
#include "core/Context.h"
#include "core/StepTimer.h"

class RenderSystem : public ISystem {
public:
	RenderSystem(Context& context);
	~RenderSystem();

	virtual void Update() override;

private:
	Context& m_ctx;
	DX::StepTimer m_timer;
};

