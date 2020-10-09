#include "Shader.h"
#include <stdio.h>

GLuint CompileShader(const char* src, GLint type)
{
	GLuint shader = glCreateShader(type);

	glShaderSource(shader, 1, &src, NULL);

	glCompileShader(shader);
	GLint compiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	GLint infoLen = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

	if (infoLen > 1)
	{
		printf("%s during shader compilation.\n ", compiled == GL_TRUE ? "Warning" : "Error");
		char* buf = new char[infoLen];
		glGetShaderInfoLog(shader, infoLen, NULL, buf);
		printf("Compilation log: %s\n", buf);
		delete[] buf;
	}

	return shader;
}

void Shader::Create(const char* vsrc, const char* fsrc)
{
	int vertex_shader_handle = CompileShader(vsrc, GL_VERTEX_SHADER);
	int fragment_shader_handle = CompileShader(fsrc, GL_FRAGMENT_SHADER);

	m_program = glCreateProgram();

	glAttachShader(m_program, vertex_shader_handle);
	glAttachShader(m_program, fragment_shader_handle);

	glLinkProgram(m_program);

	int linked;
	glGetProgramiv(m_program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		GLint infoLen = 0;
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &infoLen);
		if (infoLen > 1)
		{
			char* buf = new char[infoLen];
			glGetProgramInfoLog(m_program, infoLen, NULL, buf);
			printf("Linking error: \n%s\n", buf);
			delete[] buf;
		}
	}

	glDetachShader(m_program, vertex_shader_handle);
	glDetachShader(m_program, fragment_shader_handle);

	glDeleteShader(vertex_shader_handle);
	glDeleteShader(fragment_shader_handle);

	m_attrib_pos = glGetAttribLocation(m_program, "a_position");
	m_attrib_normal = glGetAttribLocation(m_program, "a_normal");
	m_attrib_uv = glGetAttribLocation(m_program, "a_uv");

	u_modelView = glGetUniformLocation(m_program, "u_modelView");
	u_projection = glGetUniformLocation(m_program, "u_projection");

	u_ambientProduct = glGetUniformLocation(m_program, "u_ambientProduct");
	u_diffuseProduct = glGetUniformLocation(m_program, "u_diffuseProduct");
	u_specularProduct = glGetUniformLocation(m_program, "u_specularProduct");
	u_shininess = glGetUniformLocation(m_program, "u_shininess");
	u_lightPos = glGetUniformLocation(m_program, "u_lightPos");
	u_lightPos2 = glGetUniformLocation(m_program, "u_lightPos2");
	u_lightPos3 = glGetUniformLocation(m_program, "u_lightPos3");
	u_texture = glGetUniformLocation(m_program, "u_texture");
}
