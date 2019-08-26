#include "pch.h"
#include "ModelLoading.h"

namespace exemple {
	ModelLoading::ModelLoading(DXObjects& dxObjects)
		: m_dxo(dxObjects), m_modelFactory("res/models/cube/Cube.gltf")
	{
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
