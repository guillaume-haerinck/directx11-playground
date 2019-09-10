#pragma once

#include "ISystem.h"
#include "core/Context.h"

class RenderSystem : public ISystem {
public:
	RenderSystem(Context& context);
	virtual ~RenderSystem();

	void Update() override;

private:
	Context& m_ctx;
};

