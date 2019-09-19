#pragma once

namespace comp {
	enum PipelineShaderIndex {
		VS = 0,
		PS,
		_MAX_SHADER
	};

	/**
	 * @brief 
	 */
	struct Pipeline {
		std::array<bool, PipelineShaderIndex::_MAX_SHADER> hasShader;
		unsigned int vsIndex;
		unsigned int psIndex;
		unsigned int gsIndex;
	};
};
