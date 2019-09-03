#pragma once

#include "Material.h"
#include "Shader.h"
#include "Mesh.h"

namespace comp {
	struct Model {
		Model(VertexBuffer vb, IndexBuffer ib) : vb(vb), ib(ib) {}

		VertexBuffer vb;
		IndexBuffer ib;
	};
};
