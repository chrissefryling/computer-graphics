#include "DebugRenderer.h"
#include "ShaderUtils.h"
#include <tuple>


DebugRenderer::DebugRenderer()
{
}

DebugRenderer::~DebugRenderer()
{

}

void DebugRenderer::Init()
{
	const char* vertex_shader_src = R"(
		attribute vec3 a_position;
		attribute vec4 a_color;

		uniform mat4 u_transform;

		varying vec4 v_color;

		void main()
		{
			v_color = a_color;
			gl_Position = u_transform * vec4(a_position, 1.0);;
		}
	)";

	const char* fragment_shader_src = R"(
		varying vec4 v_color;

		void main()
		{
			gl_FragColor = v_color;
		}
	)";

	m_program = MakeProgram(vertex_shader_src, fragment_shader_src);

	m_attrib_pos = glGetAttribLocation(m_program, "a_position");
	m_attrib_color = glGetAttribLocation(m_program, "a_color");
	m_uniform_transform = glGetUniformLocation(m_program, "u_transform");

	m_vertexIt = 0;
}

void DebugRenderer::PushVertex(const glm::vec3& p, const glm::ivec3& color)
{
	PushVertex(p, glm::ivec4(color, 255));
}

void DebugRenderer::PushVertex(const glm::vec3& p, const glm::ivec4& color)
{
	Vertex v;
	v.x = p.x;
	v.y = p.y;
	v.z = p.z;
	v.r = static_cast<unsigned char>(color.r);
	v.g = static_cast<unsigned char>(color.g);
	v.b = static_cast<unsigned char>(color.b);
	v.a = static_cast<unsigned char>(color.a);
	m_vertexArray.push_back(v);
}

void DebugRenderer::EmitLineStrip()
{
	for (int i = m_vertexIt + 1, s = m_vertexArray.size(); i < s; ++i)
	{
		m_lineIndexArray.push_back(m_vertexIt);
		m_lineIndexArray.push_back(i);
		m_vertexIt = i;
	}
	m_vertexIt = m_vertexArray.size();
}

void DebugRenderer::EmitLines()
{
	for (int i = m_vertexIt, s = m_vertexArray.size(); i < s; ++i)
	{
		m_lineIndexArray.push_back(i);
	}
	m_vertexIt = m_vertexArray.size();
}

void DebugRenderer::EmitPoints()
{
	for (int i = m_vertexIt, s = m_vertexArray.size(); i < s; ++i)
	{
		m_pointIndexArray.push_back(i);
	}
	m_vertexIt = m_vertexArray.size();
}

void DebugRenderer::EmitTriangles()
{
	for (int i = m_vertexIt, s = m_vertexArray.size(); i < s; ++i)
	{
		m_trianglesIndexArray.push_back(i);
	}
	m_vertexIt = m_vertexArray.size();
}

void DebugRenderer::Draw(const glm::mat4& transform)
{
	GLint id;
	glGetIntegerv(GL_CURRENT_PROGRAM,&id);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glUseProgram(m_program);
	glUniformMatrix4fv(m_uniform_transform, 1, GL_FALSE, &transform[0][0]);

	glEnableVertexAttribArray(m_attrib_pos);
	glVertexAttribPointer(m_attrib_pos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), &m_vertexArray[0]);

	glEnableVertexAttribArray(m_attrib_color);
	glVertexAttribPointer(m_attrib_color, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(Vertex), reinterpret_cast<float*>(&m_vertexArray[0]) + 3);

	if (m_lineIndexArray.size() > 1)
	{
		glDrawElements(GL_LINES, (GLsizei)m_lineIndexArray.size(), GL_UNSIGNED_INT, &m_lineIndexArray[0]);
	}
	if (m_pointIndexArray.size() > 0)
	{
		glDrawElements(GL_POINTS, (GLsizei)m_pointIndexArray.size(), GL_UNSIGNED_INT, &m_pointIndexArray[0]);
	}
	if (m_trianglesIndexArray.size() > 2)
	{
		glDrawElements(GL_TRIANGLES, (GLsizei)m_trianglesIndexArray.size(), GL_UNSIGNED_INT, &m_trianglesIndexArray[0]);
	}

	glDisableVertexAttribArray(m_attrib_color);
	glDisableVertexAttribArray(m_attrib_pos);

	glUseProgram(id);

	m_vertexIt = 0;
	m_lineIndexArray.resize(0);
	m_pointIndexArray.resize(0);
	m_trianglesIndexArray.resize(0);
	m_vertexArray.resize(0);
}
