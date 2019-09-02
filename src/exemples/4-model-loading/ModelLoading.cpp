#include "pch.h"
#include "ModelLoading.h"

#include "factories/components/ModelFactory.h"

namespace exemple {
	ModelLoading::ModelLoading(Context& context) : m_ctx(context) {
		ModelFactory modelFactory;
		comp::Model model = modelFactory.CreateModel("res/models/cube/Cube.gltf");
	}

	ModelLoading::~ModelLoading()
	{
	}

	void ModelLoading::Update()
	{
	}

	void ModelLoading::ImGuiUpdate()
	{
	}
}
