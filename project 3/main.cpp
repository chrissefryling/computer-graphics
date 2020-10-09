#include "Application.h"

#include <GLFW/glfw3.h>
#include <memory>
#include <chrono>
#include "imgui.h"
#include "examples/opengl3_example/imgui_impl_glfw_gl3.h"

int main()
{
	GLFWwindow* window;

	/* Initialize the library */
	glfwInit();

    int width = 640;
    int height = 640;
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	ImGui_ImplGlfwGL3_Init(window, false);

	{
		std::shared_ptr<Application> app = std::make_shared<Application>();
		
		app->Resize(width * 2, height * 2);
		
		glfwSetWindowUserPointer(window, app.get());

		glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
		{
			Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
			app->Resize(width * 2, height * 2);
		});

		glfwSetKeyCallback(window, [](GLFWwindow*, int key, int, int action, int mods)
		{
			ImGuiIO& io = ImGui::GetIO();
			if (action == GLFW_PRESS)
				io.KeysDown[key] = true;
			if (action == GLFW_RELEASE)
				io.KeysDown[key] = false;

			io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
			io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
			io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
			io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
		});

		glfwSetCharCallback(window, [](GLFWwindow*, unsigned int c)
		{
			ImGuiIO& io = ImGui::GetIO();
			io.AddInputCharacter((unsigned short)c);
		});

		glfwSetScrollCallback(window, [](GLFWwindow*, double /*xoffset*/, double yoffset)
		{
			ImGuiIO& io = ImGui::GetIO();
			io.MouseWheel += (float)yoffset * 2.0f;
		});

		glfwSetMouseButtonCallback(window, [](GLFWwindow*, int button, int action, int /*mods*/)
		{
			ImGuiIO& io = ImGui::GetIO();

			if (button >= 0 && button < 3)
			{
				io.MouseDown[button] = action == GLFW_PRESS;
			}
		});

		auto start = std::chrono::steady_clock::now();

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			auto current_timestamp = std::chrono::steady_clock::now();

			std::chrono::duration<float> elapsed_time = (current_timestamp - start);

			ImGui_ImplGlfwGL3_NewFrame();

			app->Draw(elapsed_time.count());

			ImGui::Render();

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

		glfwSetWindowSizeCallback(window, nullptr);
		ImGui_ImplGlfwGL3_Shutdown();
	}

	glfwTerminate();
	return 0;
}
