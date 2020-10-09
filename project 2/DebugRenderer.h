#pragma once
#include <glm/glm.hpp>
#include <vector>


class DebugRenderer
{
	struct Vertex
	{
		float x, y, z;
		unsigned char r, g, b, a;
	};

public:
	static constexpr glm::ivec3 Red() { return glm::ivec3(0xaa, 0x00, 0x00); }
	static constexpr glm::ivec3 Green() { return glm::ivec3(0x00, 0xaa, 0x00); }
	static constexpr glm::ivec3 Yellow() { return glm::ivec3(0xaa, 0x55, 0x00); }
	static constexpr glm::ivec3 Blue() { return glm::ivec3(0x00, 0x00, 0xaa); }
	static constexpr glm::ivec3 Magenta() { return glm::ivec3(0xaa, 0x00, 0xaa); }
	static constexpr glm::ivec3 Cyan() { return glm::ivec3(0x00, 0xaa, 0xaa); }
	static constexpr glm::ivec3 White() { return glm::ivec3(0xaa, 0xaa, 0xaa); }

	DebugRenderer();
	~DebugRenderer();

	void Init();

	void PushVertex(const glm::vec3& p0, const glm::ivec3& color);

	void PushVertex(const glm::vec3& p0, const glm::ivec4& color);

	void EmitLines();

	void EmitLineStrip();

	void EmitPoints();

	void EmitTriangles();

	void Draw(const glm::mat4& viewProjection);

private:
	std::vector<Vertex> m_vertexArray;
	std::vector<int> m_pointIndexArray;
	std::vector<int> m_lineIndexArray;
	std::vector<int> m_trianglesIndexArray;

	unsigned int m_attrib_pos;
	unsigned int m_attrib_color;
	unsigned int m_uniform_transform;
	int m_program;
	int m_vertexIt;
};


inline void DrawCoordinateSystemOrigin(DebugRenderer& debug, const glm::mat4& transform, float size=20.0f)
{
	debug.PushVertex(glm::vec3(0.0f, 0.0f, 0.0f), DebugRenderer::Red());
	debug.PushVertex(glm::vec3(size, 0.0f, 0.0f), DebugRenderer::Red());
	debug.PushVertex(glm::vec3(0.0f, 0.0f, 0.0f), DebugRenderer::Green());
	debug.PushVertex(glm::vec3(0.0f, size, 0.0f), DebugRenderer::Green());
	debug.PushVertex(glm::vec3(0.0f, 0.0f, 0.0f), DebugRenderer::Blue());
	debug.PushVertex(glm::vec3(0.0f, 0.0f, size), DebugRenderer::Blue());
	debug.EmitLines();
	debug.Draw(transform);
}


inline void DrawCross(DebugRenderer& debug, const glm::ivec3& color, const glm::mat4& transform, float size=20.0f)
{
	debug.PushVertex(glm::vec3(-size, 0.0f, 0.0f), color);
	debug.PushVertex(glm::vec3(size, 0.0f, 0.0f), color);
	debug.PushVertex(glm::vec3(0.0f, -size, 0.0f), color);
	debug.PushVertex(glm::vec3(0.0f, size, 0.0f), color);
	debug.PushVertex(glm::vec3(0.0f, 0.0f, -size), color);
	debug.PushVertex(glm::vec3(0.0f, 0.0f, size), color);
	debug.EmitLines();
	debug.Draw(transform);
}
