#pragma once

#include "core/Context.h"

class MaterialFactory {
public:
	MaterialFactory(Context& context);
	~MaterialFactory();

private:
	Context& m_ctx;
};
