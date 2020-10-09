#include <string>
#include <glm/glm.hpp>

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 uv;
};

class Object
{
public:
	void Load(std::string path);

	void MakeBox();

	void Bind();
	void Draw();
	void UnBind();
private:
	unsigned int m_vertexBufferObject;
	unsigned int m_indexBufferObject;
	unsigned int m_indexSize;
};
