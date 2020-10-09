#include "Object.h"
#include <GL/gl3w.h>
#include <vector>
#include <iostream>
#include <fstream>


Object::Object(): m_vertexBufferObject(0), m_indexBufferObject(0), m_indexSize(0)
{
}

Object::~Object()
{
	glDeleteBuffers(1, &m_vertexBufferObject);
	glDeleteBuffers(1, &m_indexBufferObject);
}

ObjectPtr Object::Create()
{
	return ObjectPtr(new Object());
}

void Object::Load(const std::string& path)
{
	glGenBuffers(1, &m_vertexBufferObject);
	glGenBuffers(1, &m_indexBufferObject);

	std::vector<Vertex> vertices;
	std::vector<int> indices;
	std::ifstream file(path);
	std::string str;
    
    if (!file)
    {
        file = std::ifstream("../" + path);

        if (!file)
        {
            printf("Could not find file %s", path.c_str());
            exit(-1);
        }
    }

	while (std::getline(file, str))
	{
		// Read mesh data from file and populate vertices and indices
		Vertex v;
		v.pos = glm::vec3();

		// Hint:
		// Use sscanf(str.c_str(), "v %f %f %f", &v.pos.x, &v.pos.y, &v.pos.z); to read vertex
		// Use sscanf(str.c_str(), "f %d %d %d", &a, &b, &c); to read face data
		// Do not forget to substruct 1 from index data, since in obj index data starts from 1.

		// Refer to the slides and example code. Everything is already written there

		// WRITE CODE HERE
        // vertices
        if(strncmp(str.c_str(), "v ", 2) == 0){
            sscanf(str.c_str(), "v %f %f %f", &v.pos.x, &v.pos.y, &v.pos.z);
            vertices.push_back(v);
        }
        // indices
        else if(strncmp(str.c_str(), "f ", 2) == 0){
            int a,b,c;
            sscanf(str.c_str(), "f %d %d %d", &a, &b, &c);
            indices.push_back(a-1);
            indices.push_back(b-1);
            indices.push_back(c-1);
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

		// Knowing vertices p1, p2, and p3 compute the normal

		// WRITE CODE HERE
//		vertices[i].normal ?;
//		vertices[j].normal ?;
//		vertices[k].normal ?;
        auto norm = cross(p2 - p1, p3 - p2);
        vertices[i].normal = norm;
        vertices[j].normal = norm;
        vertices[k].normal = norm;
        
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


// The code below creates a box mesh
// Do not change this function
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
