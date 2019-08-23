#pragma once

class Sphere {
public:
	Sphere();
	~Sphere();

	std::vector<XMFLOAT3> GetVertices() const { return m_vertices; }
	std::vector<unsigned int> GetIndices() const { return m_indices; }

private:
	std::vector<XMFLOAT3> m_vertices;
	std::vector<unsigned int> m_indices;
};
