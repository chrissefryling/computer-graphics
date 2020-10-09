#pragma once
#include <string>
#include <glm/glm.hpp>
#include <memory>


struct Vertex
{
	glm::vec3 pos;
	glm::vec3 normal;
};

class Object;
typedef std::shared_ptr<Object> ObjectPtr;

class Object
{
public:
	static ObjectPtr Create();

	Object( const Object& ) = delete; // non construction-copyable
	Object& operator=( const Object& ) = delete; // non copyable

	~Object();

	void Load(const std::string& path);

	void MakeBox();

	void Bind();
	void Draw();
	void UnBind();
private:
	Object();

	unsigned int m_vertexBufferObject;
	unsigned int m_indexBufferObject;
	unsigned int m_indexSize;
};
