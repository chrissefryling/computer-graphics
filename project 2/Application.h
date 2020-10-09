#pragma once
#include "Object.h"
#include "DebugRenderer.h"

inline void* ToVoidPointer(int offset)
{
	size_t offset_ = static_cast<size_t>(offset);
	return reinterpret_cast<void*>(offset_);
}


class Application
{
public:
	Application();
	~Application();

	void Draw(float time);

private:
	void DrawMesh(const ObjectPtr&);

	ObjectPtr m_teapot;
	ObjectPtr m_box;

	DebugRenderer m_debug;

	glm::mat4 m_viewProjection;

	unsigned int m_program;
	unsigned int m_attrib_pos;
	unsigned int m_attrib_normal;
	unsigned int m_uniform_transform;
	unsigned int m_uniform_viewProjection;
	unsigned int m_uniform_color;
	unsigned int m_uniform_ambient;
	unsigned int m_uniform_light_pos;
};
