#pragma once

#include "Material.h"
#include "Shader.h"
#include "Mesh.h"

namespace comp {
	struct Model {
		std::vector<Mesh> meshes;
	};
};
