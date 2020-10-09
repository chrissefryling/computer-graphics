#include "Application.h"
#include <GL/gl3w.h>
#include <stdio.h>
#include <glm/glm.hpp>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <glm/gtc/matrix_transform.hpp>

#include "imgui.h"
#include "examples/opengl3_example/imgui_impl_glfw_gl3.h"


struct Texture
{
	glm::vec<3, char>* buff;
	int width;
	int height;
};

Texture loadtexture(const char* path)
{
	Texture t;
	std::ifstream  tgafile(path, std::ios::binary);
	char buff[18];
	tgafile.read(buff, 18);
	t.width = *((short*)&buff[0xc]);
	t.height = *((short*)&buff[0xe]);
	int size = 3 * sizeof(char) * t.width * t.height;
	t.buff = new glm::vec<3, char>[t.width * t.height];
	tgafile.read((char*)t.buff, size);
	return t;
}

Application::Application()
{
	gl3wInit();

	const char* OpenGLversion = (const char*)glGetString(GL_VERSION);
	const char* GLSLversion = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);

	printf("OpenGL %s GLSL: %s", OpenGLversion, GLSLversion);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	const char* vertex_shader_src = R"(
		attribute vec3 a_position;
		attribute vec3 a_normal;
		attribute vec2 a_uv;

		uniform mat4 u_modelView;
		uniform mat4 u_projection;

		varying vec3 v_pos;
		varying vec2 v_uv;
    
        uniform vec3 u_lightPos;
        uniform vec3 u_lightPos2;
        uniform vec3 u_lightPos3;
        varying vec3 v_L;
        varying vec3 v_L2;
        varying vec3 v_L3;
        varying vec3 v_E;
        varying vec3 v_N;

		void main()
		{
    //p2 & p3 are for spotlights
			vec4 pos = u_modelView * vec4(a_position, 1.0);
            vec4 pos1 = u_modelView * vec4(u_lightPos, 0.0);
            vec4 p2 = u_modelView * vec4(u_lightPos2, 1.0);
            vec4 p3 = u_modelView * vec4(u_lightPos3, 1.0);
			v_pos = a_position.xyz;

            v_L = pos1.xyz;
            v_L2 = p2.xyz - pos.xyz;
            v_L3 = p3.xyz - pos.xyz;
            v_E = -pos.xyz;
            v_N = (u_modelView * vec4(a_normal, 0.0)).xyz;
    
			v_uv = a_uv;
            //compute LNE here - parse to fragment shader
			gl_Position = u_projection * pos;
		}
	)";

	const char* fragment_shader_src = R"(
		// light and material properties
		uniform sampler2D u_texture;
		varying vec2 v_uv;
        uniform vec3 u_ambientProduct;
        uniform vec3 u_diffuseProduct;
        uniform vec3 u_specularProduct;
       // uniform vec3 u_direction;
        uniform float u_shininess;
        varying vec3 v_L;
        varying vec3 v_E;
        varying vec3 v_N;
        varying vec3 v_L2;
        varying vec3 v_L3;
    
		varying vec3 v_pos;

		void main()
		{

            
            vec3 L = normalize(v_L);
    //for spotlights below
            vec3 L2 = normalize(v_L2);
            vec3 L3 = normalize(v_L3);
    
            vec3 E = normalize(v_E);
            vec3 N = normalize(v_N);
            vec3 H = normalize(L + E);
            float Kd = max(dot(L, N), 0.0);
            vec3 diffuse = Kd * u_diffuseProduct; float Ks = pow(max(dot(N, H), 0.0), u_shininess);
            vec3 specular = Ks * u_specularProduct;
            if (dot(L, N) < 0.0)
                specular = vec3(0.0, 0.0, 0.0);
    
        //for spotlights 1 & 2 [ l2 and l3 ]
            float Kd2 = max(dot(L2, N), 0.0) / (length(v_L2)*length(v_L2))*0.01;
            vec3 diffuse2 = Kd2 * vec3(1.0, 0.0, 0.0);
    
            float Kd3 = max(dot(L3, N), 0.0) / (length(v_L3)*length(v_L3))*0.01;
            vec3 diffuse3 = Kd3 * vec3(0.0, 1.0, 0.0);
            
            
            //vec3 lightDir = normalize(-light.u_direction);
			vec3 albido = pow(texture2D(u_texture, v_uv).bgr, vec3(2.2));
    
            vec3 color = (u_ambientProduct + diffuse + diffuse2 + diffuse3 + specular) * albido;

			gl_FragColor = vec4(pow(color, vec3(1.0/2.2)), 1.0);
		}
	)";

	m_shader.Create(vertex_shader_src, fragment_shader_src);
	
	m_obj.Load("../../LeePerrySmith.obj");

	Texture t = loadtexture("../../albido.tga");

	glGenTextures(1, &m_textureHandle);
	glBindTexture(GL_TEXTURE_2D, m_textureHandle);

	int size = t.width * t.height * 3;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, t.width, t.height, 0, GL_RGB, GL_UNSIGNED_BYTE, t.buff);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

inline void* ToVoidPointer(int offset)
{
	size_t offset_ = static_cast<size_t>(offset);
	return reinterpret_cast<void*>(offset_);
}

void Application::Draw(float time)
{
	ImGui::Begin("IntroductionToComputerGraphics");
    
    
    static glm::vec3 ambient = glm::vec3(0.1f, 0.1f, 0.1f);
    ImGui::ColorEdit3("ambient", (float*)&ambient);
    
    static glm::vec3 diffuse = glm::vec3(0.1f, 0.1f, 0.1f);
    ImGui::ColorEdit3("diffuse", (float*)&diffuse);
    
    static glm::vec3 specular = glm::vec3(0.1f, 0.1f, 0.1f);
    ImGui::ColorEdit3("specular", (float*)&specular);
    
    static glm::vec3 lightpos = glm::vec3(100.0f, 100.0f, 0.1f);
    ImGui::SliderFloat3("lightpos", (float*)&lightpos, -100.f, 100.0f);
    
    static float shiness = 100.0f;
    ImGui::SliderFloat("shiness", (float*)&shiness, 0.0f, 300.0f);
    
   // ImGui::SliderFloat("Gamma", (float*)&m_gamma, 1.0f, 3.0f);
    
	ImGui::End();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     
	glViewport(0, 0, m_width, m_height);

	float aspect = m_width / (float)m_height;

	float view_height = 2.2f;
	float view_width = aspect * view_height;

	float left = -view_width / 2.0f;
	float right = view_width / 2.0f;
	float top = view_height / 2.0f;
	float bottom = - view_height / 2.0f;

	glUseProgram(m_shader.m_program);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textureHandle);

	glUniform1i(m_shader.u_texture, 0);

    // controls how much the object spins
	float camera_rotation = -time * 5;

	float light2_rotation = -time * 1.1;

	glm::mat4 projection = glm::ortho(-0.3f * aspect, 0.3f * aspect, -0.3f, 0.3f, -3.0f, 3.0f);
	glm::vec3 camera_pos = glm::vec3(1.0, 0.5, 1.0);
	camera_pos = glm::rotate(glm::mat4(1.0), camera_rotation, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(camera_pos, 1.0);

	glm::mat4 view = glm::lookAt(camera_pos, glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0));
//lightingShader.setVec3("light.u_direction", -0.2f, -1.0f, -0.3f); 
	glUniformMatrix4fv(m_shader.u_projection, 1, GL_FALSE, &projection[0][0]);

	glm::mat4 modelView = view;
	glUniformMatrix4fv(m_shader.u_modelView, 1, GL_FALSE, &modelView[0][0]);
    glUniform3fv(m_shader.u_ambientProduct, 1, (float*)&ambient);
    glUniform3fv(m_shader.u_diffuseProduct, 1, (float*)&diffuse);
    glUniform3fv(m_shader.u_specularProduct, 1, (float*)&specular);
    glUniform3fv(m_shader.u_lightPos, 1, (float*)&lightpos);
    glUniform1fv(m_shader.u_shininess, 1, (float*)&shiness);
    
    glm::vec3 Light1 = glm::vec3(cos(time * 3.0) * 0.2f, -0.2f, sin(time * 3.0) * 0.2f);
    glm::vec3 Light2 = glm::vec3(cos(time * 5.0) * 0.2f, 0.1f, sin(time * 5.0) * 0.2f);
        
    glUniform3fv(m_shader.u_lightPos2, 1, (float*)&Light1);
    glUniform3fv(m_shader.u_lightPos3, 1, (float*)&Light2);

	m_obj.Bind();

	glEnableVertexAttribArray(m_shader.m_attrib_pos);
	glVertexAttribPointer(m_shader.m_attrib_pos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), ToVoidPointer(0));

	glEnableVertexAttribArray(m_shader.m_attrib_normal);
	glVertexAttribPointer(m_shader.m_attrib_normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), ToVoidPointer(sizeof(glm::vec3)));

	glEnableVertexAttribArray(m_shader.m_attrib_uv);
	glVertexAttribPointer(m_shader.m_attrib_uv, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), ToVoidPointer(2 * sizeof(glm::vec3)));

	m_obj.Draw();

	glDisableVertexAttribArray(m_shader.m_attrib_pos);
	glDisableVertexAttribArray(m_shader.m_attrib_normal);
	glDisableVertexAttribArray(m_shader.m_attrib_uv);

	m_obj.UnBind();
}


void Application::Resize(int width, int height)
{
	m_width = width;
	m_height = height;
}
