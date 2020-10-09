#include "Object.h"
#include <glm/glm.hpp>
#include <GL/gl3w.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

void Object::Load(std::string path)
{
	glGenBuffers(1, &m_vertexBufferObject);
	glGenBuffers(1, &m_indexBufferObject);

	std::map<int, int> vertex_cache;
	std::vector<Vertex> vertices;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> uv_coordinates;
	std::vector<int> indices;

	std::ifstream file(path);
	std::string str;
	glm::vec3 pos;
	glm::vec3 uv;
	int current_index = 0;

	while (std::getline(file, str))
	{
		if (strncmp(str.c_str(), "v ", 2) == 0)
		{
			sscanf(str.c_str(), "v %f %f %f", &pos.x, &pos.y, &pos.z);
			positions.push_back(pos);
		}
		else if (strncmp(str.c_str(), "vt ", 3) == 0)
		{
			sscanf(str.c_str(), "vt %f %f", &uv.x, &uv.y);
			uv_coordinates.push_back(uv);
		}
		else if (strncmp(str.c_str(), "f ", 2) == 0)
		{
			int pos[4] = { 0, 0, 0 , 0 };
			int uv[4] = { 0, 0, 0, 0 };
			sscanf(str.c_str(), "f %d/%d %d/%d %d/%d %d/%d", pos, uv, pos + 1, uv + 1, pos + 2, uv + 2, pos + 3, uv + 3);
			int ind[4] = { 0, 0, 0, 0 };
			for (int i = 0; i < 4; ++i)
			{
				auto key = pos[i];
				auto it = vertex_cache.find(key);
				if (it != vertex_cache.end())
				{
					ind[i] = it->second;
				}
				else
				{
					vertex_cache[key] = current_index;
					ind[i] = current_index;
					++current_index;
					Vertex v;
					v.pos = positions[pos[i] - 1];
					v.uv = uv_coordinates[uv[i] - 1];
					v.normal = glm::vec3(0);
					vertices.push_back(v);
				}
			}
			indices.push_back(ind[0]);
			indices.push_back(ind[1]);
			indices.push_back(ind[2]);
			indices.push_back(ind[0]);
			indices.push_back(ind[2]);
			indices.push_back(ind[3]);
		}
	}

	m_indexSize = indices.size();

	for (int f = 0; f < m_indexSize / 3; ++f)
	{
		int i = indices[3 * f + 0];
		int j = indices[3 * f + 1];
		int k = indices[3 * f + 2];
		glm::vec3 p1 = vertices[i].pos;
		glm::vec3 p2 = vertices[j].pos;
		glm::vec3 p3 = vertices[k].pos;
		glm::vec3 a = p1 - p3;
		glm::vec3 b = p2 - p3;
		glm::vec3 c = glm::cross(a, b);
		vertices[i].normal += c;
		vertices[j].normal += c;
		vertices[k].normal += c;
	}

	for (int i = 0; i < vertices.size(); ++i)
	{
		vertices[i].normal = glm::normalize(vertices[i].normal);
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


// The code below creates a box mesh
void Object::MakeBox()
{
	glGenBuffers(1, &m_vertexBufferObject);
	glGenBuffers(1, &m_indexBufferObject);

	std::vector<Vertex> vertices;
	for (int i = 0; i < 6; ++i)
	{
		glm::vec3 dir(0.0f);
		glm::vec3 dir2(0.0f);
		glm::vec3 dir3(0.0f);
		dir[i % 3] = i > 2 ? 1.0f : -1.0f;
		dir2[(i + 1) % 3] = i > 2 ? 1.0f : -1.0f;
		dir3[(i + 2) % 3] = i > 2 ? 1.0f : -1.0f;
		vertices.push_back({ dir - dir2 - dir3, dir });
		vertices.push_back({ dir - dir2 + dir3, dir });
		vertices.push_back({ dir + dir2 - dir3, dir });
		vertices.push_back({ dir + dir2 + dir3, dir });
	}
	std::vector<int> indices;
	for (int i = 0; i < 6; ++i)
	{
		indices.push_back(i * 4 + 0);
		indices.push_back(i * 4 + 1);
		indices.push_back(i * 4 + 2);
		indices.push_back(i * 4 + 1);
		indices.push_back(i * 4 + 2);
		indices.push_back(i * 4 + 3);
	}

	m_indexSize = indices.size();

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Object::Draw()
{
	glDrawElements(GL_TRIANGLES, m_indexSize, GL_UNSIGNED_INT, 0);
}

void Object::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferObject);
}

void Object::UnBind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
