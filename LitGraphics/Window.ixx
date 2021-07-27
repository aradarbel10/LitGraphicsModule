module;
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <map>
#include <algorithm>

#include <iostream>

export module Window;

import Tuple;
import Polygon;
import Shader;

namespace lgm {

	export enum Key {
		Unknown = -1,
		Space = 32, Apostrophe = 39, Comma = 44, Minus, Period, Slash,
		Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
		Semicolon = 59, Equal = 61,
		A = 65, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		LeftBracket, Backslash, RightBracket,
		GraveAccent = 96,
		World1 = 161, World2,
		Escape = 256, Enter, Tab, Backspace, Insert, Delete, Right, Left, Down, Up, PageUp, PageDown, Home, End,
		CapsLock = 280, ScrollLock, NumLock, PrintScreen, Pause,
		F1 = 290, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24, F25,
		Numpad0 = 320, Numpad1, Numpad2, Numpad3, Numpad4, Numpad5, Numpad6, Numpad7, Numpad8, Numpad9,
		NumpadDecimal, NumpadDivide, NumpadMultiply, NumpadSubtract, NumpadAdd, NumpadEnter, NumpadEqual,
		LeftShift = 340, LeftCtrl, LeftAlt, LeftSuper, RightShift, RightCtrl, RightAlt, RightSuper, Menu
	};

	export class Window {
	public:

		static void keyEventCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
			if (action == GLFW_PRESS) {
				refMap[window]->keyState[key] = true;
			} else if (action == GLFW_RELEASE) {
				refMap[window]->keyState[key] = false;
			}
		}

		Window() = delete;

		Window(const vector2i& size, const std::string& title) {
			// initializing GLFW
			glfwInit();

			// tell GLFW exactly which openGL version & profile we will use
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			this->size = size;
			window = glfwCreateWindow(size.x, size.y, title.c_str(), NULL, NULL);
			refMap.emplace(std::make_pair(window, this));

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

			glfwSetKeyCallback(window, keyEventCallback);
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

		void draw(lgm::Polygon& p, bool drawMesh = false) {
			p.draw(*polygonShader.get(), size, drawMesh);
		}

		float getDeltaTime() {
			return dtime;
		}

		bool isPressed(int key) {
			if (!keyState.contains(key)) keyState[key] = false;
			return keyState[key];
		}

		~Window() {
			// remove from refMap
			refMap.erase(window);
			// once done, kill all windows we opened
			glfwDestroyWindow(window);
			// terminate GLFW
			glfwTerminate();
		}

	private:

		static std::map<GLFWwindow*, Window*> refMap;

		lgm::vector2i size{ 0, 0 };

		GLFWwindow* window;
		lgm::color background{0.78f, 0.78f, 0.78f, 1.0f};

		std::unique_ptr<lgm::ShaderProgram> polygonShader;
		float dtime = 0;

		std::map<int, bool> keyState;

	};

	std::map<GLFWwindow*, Window*> Window::refMap;
}