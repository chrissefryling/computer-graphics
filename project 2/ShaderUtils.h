#include <GL/gl3w.h>
#include <stdio.h>


inline GLuint CompileShader(const char* src, GLint type)
{
	GLuint shader = glCreateShader(type);

	glShaderSource(shader, 1, &src, nullptr);

	glCompileShader(shader);
	GLint compiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	GLint infoLen = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

	if (infoLen > 1)
	{
		printf("%s during shader compilation.\n ", compiled == GL_TRUE ? "Warning" : "Error");
		char* buf = new char[infoLen];
		glGetShaderInfoLog(shader, infoLen, nullptr, buf);
		printf("Compilation log: %s\n", buf);
		delete[] buf;
	}

	return shader;
}

inline GLuint MakeProgram(const char* vertex_shader_src, const char* fragment_shader_src)
{
	int vertex_shader_handle = CompileShader(vertex_shader_src, GL_VERTEX_SHADER);
	int fragment_shader_handle = CompileShader(fragment_shader_src, GL_FRAGMENT_SHADER);

	GLuint program = glCreateProgram();

	glAttachShader(program, vertex_shader_handle);
	glAttachShader(program, fragment_shader_handle);

	glLinkProgram(program);

	int linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		GLint infoLen = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
		if (infoLen > 1)
		{
			char* buf = new char[infoLen];
			glGetProgramInfoLog(program, infoLen, NULL, buf);
			printf("Linking error: \n%s\n", buf);
			delete[] buf;
		}
	}

	glDetachShader(program, vertex_shader_handle);
	glDetachShader(program, fragment_shader_handle);

	glDeleteShader(vertex_shader_handle);
	glDeleteShader(fragment_shader_handle);

	return program;
}
