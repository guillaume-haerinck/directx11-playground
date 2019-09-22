#pragma once

namespace comp {
	enum PipelineShaderIndex {
		VS = 0,
		PS,
		GS,
		_MAX_SHADER
	};

	/**
	 * @brief 
	 */
	struct Pipeline {
		std::array<bool, PipelineShaderIndex::_MAX_SHADER> hasShader = { true, true, false };
		unsigned int vsIndex = 0;
		unsigned int psIndex = 0;
		unsigned int gsIndex = 0;
	};
};
