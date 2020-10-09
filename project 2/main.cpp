#include "Application.h"

#include <GLFW/glfw3.h>
#include <memory>
#include <chrono>

int main()
{
	GLFWwindow* window;

	/* Initialize the library */
	glfwInit();

	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_RED_BITS, 8);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1024, 1024, "Hello World", NULL, NULL);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	
	{
		std::shared_ptr<Application> app = std::make_shared<Application>();

		auto start = std::chrono::steady_clock::now();

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			auto current_timestamp = std::chrono::steady_clock::now();

			std::chrono::duration<float> elapsed_time = (current_timestamp - start);

			app->Draw(elapsed_time.count());

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;
}
