#pragma once
#include "Object.h"
#include "Shader.h"
#include <glm/glm.hpp>

struct Light
{
	glm::vec3 m_lightPos = glm::vec3(0.8, 0.8, 0.8);
	glm::vec3 m_lightColor = glm::vec3(0.2, 0.2, 0.2);
};

struct Material
{
	glm::vec3 m_albido = glm::vec3(0.8, 0.8, 0.8);
	glm::vec3 m_specular = glm::vec3(0.2, 0.2, 0.2);
	float m_shininess = 80.0f;
};

class Application
{
public:
	Application();

	void Draw(float time);
	void Resize(int width, int height);

private:	
	glm::vec3 m_ambient = glm::vec3(0.05, 0.05, 0.05);

	Light m_light0;

	Object m_obj;
	Shader m_shader;
	Material m_material;

	int m_width;
	int m_height;

	unsigned int m_textureHandle;
};
