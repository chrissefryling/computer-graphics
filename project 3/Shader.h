#include <GL/gl3w.h>

class Shader
{
public:
	void Create(const char* vsrc, const char* fsrc);

	unsigned int m_program;

	unsigned int m_attrib_pos;
	unsigned int m_attrib_normal;
	unsigned int m_attrib_uv;

	unsigned int u_modelView;
	unsigned int u_projection;
	unsigned int u_texture;

	unsigned int u_ambientProduct;
	unsigned int u_diffuseProduct;
	unsigned int u_diffuseProduct2;
	unsigned int u_specularProduct;
	unsigned int u_shininess;
	unsigned int u_lightPos;
	unsigned int u_lightPos2;
	unsigned int u_lightPos3;
};
