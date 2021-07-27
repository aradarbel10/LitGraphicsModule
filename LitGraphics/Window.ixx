module;
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

export module Window;

import Tuple;
import Polygon;
import Shader;

namespace lgm {

	export class Window {
	public:

		Window(const vector2i& size, const std::string& title) {
			// initializing GLFW
			glfwInit();

			// tell GLFW exactly which openGL version & profile we will use
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			this->size = size;
			window = glfwCreateWindow(size.x, size.y, title.c_str(), NULL, NULL);

			if (window == NULL) { // error checking
				std::cerr << "Can't create GLFW window!";
				glfwTerminate();
			}
			glfwMakeContextCurrent(window);

			// load openGL using glad
			gladLoadGL();
			glViewport(0, 0, size.x, size.y);

			// load shaders
			polygonShader = std::make_unique<lgm::ShaderProgram>("shaders/default_vertex.glsl", "shaders/default_fragment.glsl");
			polygonShader.get()->setWinSizeUniform({ (float)size.x / 2.f, (float)size.y / 2.f });
		}

		bool isOpen() {
			return !glfwWindowShouldClose(window);
		}

		void display() {
			glfwSwapBuffers(window);

			// clear window to a color
			glClearColor(background.r, background.g, background.b, background.a);
			glClear(GL_COLOR_BUFFER_BIT);

			// calculate delta time
			dtime = glfwGetTime();
			glfwSetTime(0);
		}

		void setBackgroundColor(lgm::color c) {
			background = c;
		}

		void draw(lgm::Polygon& p) {
			p.draw(*polygonShader.get(), false);
		}

		float getDeltaTime() {
			return dtime;
		}

		~Window() {
			// once done, kill all windows we opened
			glfwDestroyWindow(window);
			// terminate GLFW
			glfwTerminate();
		}

	private:

		lgm::vector2i size{ 0, 0 };

		GLFWwindow* window;
		lgm::color background{0.78f, 0.78f, 0.78f, 1.0f};

		std::unique_ptr<lgm::ShaderProgram> polygonShader;
		float dtime = 0;

	};
}