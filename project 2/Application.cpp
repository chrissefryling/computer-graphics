#include "Application.h"
#include "ShaderUtils.h"
#include <GL/gl3w.h>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>


Application::Application()
{
	gl3wInit();

	const char* OpenGLversion = (const char*)glGetString(GL_VERSION);
	const char* GLSLversion = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);

	printf("OpenGL %s GLSL: %s", OpenGLversion, GLSLversion);


	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);

	const char* vertex_shader_src = R"(
		attribute vec3 a_position;         // position in object coordinate system
		attribute vec3 a_normal;           // normal in object coordinate system

		uniform mat4 u_transform;          // transformation of object coordoinate system with respect to world coordinate system
		uniform mat4 u_viewProjection;     // projection matrix

		varying vec3 v_normal;             // normal in world coordinate system (Must be different from one in object coordinate system)
	                                       // If you just assign `v_normal = a_normal;` then you are doing it wrong. You have to use u_transform matrix
		varying vec4 v_pos;                // position of the vertex in world coordinate system

		void main()
		{
			// Write code that applies transformation to `a_position` and `a_normal`.
			// You need to use matrix u_transform

            v_pos = u_transform * vec4(a_position, 1.0); //modified
            v_normal = (u_transform * vec4(a_normal, 0.0)).xyz; // modified

			gl_Position = u_viewProjection * v_pos;
		}
	)";

	const char* fragment_shader_src = R"(
		uniform vec3 u_light_pos;
		uniform vec3 u_color;
		uniform float u_ambient;

		varying vec3 v_normal;
		varying vec4 v_pos;

		vec3 gamma(vec3 x)
		{
		    return mix(12.92 * x, 1.055 * pow(x, vec3(1.0 / 2.4)) - 0.055, step(0.0031308, x));
		}
		
		void main()
		{
			vec3 lightV = u_light_pos - v_pos.xyz;
			float diffuse = dot(normalize(v_normal), normalize(lightV));
			diffuse = max(diffuse, 0.0) / dot(lightV, lightV) * 2000.0;
			gl_FragColor = vec4(gamma(u_color * (diffuse + u_ambient)), 1.0);
		}
	)";

	m_program = MakeProgram(vertex_shader_src, fragment_shader_src);

	m_attrib_pos = glGetAttribLocation(m_program, "a_position");
	m_attrib_normal = glGetAttribLocation(m_program, "a_normal");

	m_uniform_transform = glGetUniformLocation(m_program, "u_transform");
	m_uniform_viewProjection = glGetUniformLocation(m_program, "u_viewProjection");

	m_uniform_color = glGetUniformLocation(m_program, "u_color");
	m_uniform_ambient = glGetUniformLocation(m_program, "u_ambient");
	m_uniform_light_pos = glGetUniformLocation(m_program, "u_light_pos");

	m_teapot = Object::Create();
	m_box = Object::Create();

	// After you implement loading from obj, change it so it loads obj from teapot.obj ** done
	// m_teapot->MakeBox();
	m_teapot->Load("../teapot.obj");

	m_box->MakeBox();

	m_debug.Init();


	// We set an orthogonal projection here. Do not modify this code
	glm::mat4 projection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, -200.0f, 200.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(1.0), glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0));
	m_viewProjection = projection * view;
}

Application::~Application()
{
	glDeleteProgram(m_program);
}

void Application::Draw(float time)
{
	glEnable(GL_DEPTH_TEST);

	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_program);
	glUniform1f(m_uniform_ambient, 0.02);
	glUniformMatrix4fv(m_uniform_viewProjection, 1, GL_FALSE, &m_viewProjection[0][0]);

	glm::vec3 lightpos = glm::vec3(0.0f, sin(time) * 30.0f, 0.0f);

	DrawCross(m_debug, DebugRenderer::White(), m_viewProjection * glm::translate(glm::mat4(1.0), lightpos), 1.0f);
	glUniform3fv(m_uniform_light_pos, 1, &lightpos[0]);

	// Draw world coordinate system origin
	DrawCoordinateSystemOrigin(m_debug, m_viewProjection);

	// Write your code here for the scene rendering.
	// Below is just example, you will need modify the code and write more code.
	// Do not use sin or cos functions. You are allowed to use only  glm::rotate, glm::scale, glm::translate. Use identity matrix as first argument

	// WRITE CODE HERE
    glm::mat4 rotate = glm::rotate(glm::mat4(1.0), time * 2, glm::vec3(0,1.0,0)); //rotation
    glm::mat4 shift = glm::translate(glm::mat4(1.0), glm::vec3(0,0,5.0)); // additional box that is shifted along the x axis
    glm::mat4 rotate_x = glm::rotate(glm::mat4(1.0), time *3, glm::vec3(1.0,0,0)); //replicates new box - rotates around x axis
    glm::mat4 elongate = glm::scale(glm::mat4(1.0), glm::vec3(1,1,2)); //elongates them
    
    
    
    
//creates boxes
	for (int j = 0; j < 10; ++j)
	{
		glm::mat4 teapot_coordinate_system = rotate * glm::rotate(glm::mat4(1.0f), j / 10.0f * 2.0f * 3.14f, glm::vec3(0.0, 1.0, 0.0)) *
		                                     glm::translate(glm::mat4(1.0), glm::vec3(0, 0, 30.0)) * rotate;

		DrawCoordinateSystemOrigin(m_debug, m_viewProjection * teapot_coordinate_system);

		glUniformMatrix4fv(m_uniform_transform, 1, GL_FALSE, &(teapot_coordinate_system)[0][0]);
		glUniform3f(m_uniform_color, 0.0, 1.0, 0.0);
		DrawMesh(m_teapot); //creates the box
        
        for(int i = 0; i < 10; ++i){
            glm::mat4 rotate_around = glm::rotate(glm::mat4(1.0), (i / 10.0f * 2.0f * 3.14f), glm::vec3(1.0f, 0,0));
            
        teapot_coordinate_system = rotate * glm::rotate(glm::mat4(1.0f), j / 10.0f * 2.0f * 3.14f, glm::vec3(0.0, 1.0, 0.0)) *
                                             glm::translate(glm::mat4(1.0), glm::vec3(0, 0, 30.0)) * rotate * rotate_x * rotate_around * shift * elongate;
           // DrawCoordinateSystemOrigin(m_debug, m_viewProjection * teapot_coordinate_system);
        glUniformMatrix4fv(m_uniform_transform, 1, GL_FALSE, &(teapot_coordinate_system)[0][0]);
        glUniform3f(m_uniform_color, 0.0, 1.0, 0.0);
        DrawMesh(m_box);
        }
	}
}

void Application::DrawMesh(const ObjectPtr& object)
{
	object->Bind();

	glEnableVertexAttribArray(m_attrib_pos);
	glVertexAttribPointer(m_attrib_pos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), ToVoidPointer(0));

	glEnableVertexAttribArray(m_attrib_normal);
	glVertexAttribPointer(m_attrib_normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), ToVoidPointer(sizeof(glm::vec3)));

	object->Draw();

	glDisableVertexAttribArray(m_attrib_pos);
	glDisableVertexAttribArray(m_attrib_normal);

	object->UnBind();
}
