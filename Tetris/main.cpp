#include <iostream>
#include <numbers>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

import Shader;
import VertexArray;

template <typename T>
struct vector2 {
	T x, y;
};
typedef vector2<int> vector2i;

int main() {
	// initializing GLFW
	glfwInit();

	// tell GLFW exactly which openGL version & profile we will use
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// opening a window using GLFW
	constexpr vector2i window_size = { 600, 600 };
	GLFWwindow* window = glfwCreateWindow(window_size.x, window_size.y, "Lit Graphics!", NULL, NULL);
	if (window == NULL) { // error checking
		std::cerr << "Can't create GLFW window!";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// load openGL using glad
	gladLoadGL();
	glViewport(0, 0, window_size.x, window_size.y);

	// constructing a triangle
	GLfloat traingle_vertices[] = { // equilateral triangle in the center of the window
		-0.5f, -0.5f * std::numbers::sqrt3 / 3, 0.0f,
		 0.5f, -0.5f * std::numbers::sqrt3 / 3, 0.0f,
		 0.0f,  1.0f * std::numbers::sqrt3 / 3, 0.0f
	};
	GLfloat sqaure_vertices[] = { // square in the center of the window
		-0.5f, -0.5f, //bottom left
		 0.5f, -0.5f, //bottom right
		 0.5f,  0.5f, //top right
		-0.5f,  0.5f, //top left
	};

	GLfloat sirpinski_triangle_vertices[] = { // triangle with a hole
		-0.5f,    -0.5f * std::numbers::sqrt3 / 3, 0.0f,
		 0.5f,    -0.5f * std::numbers::sqrt3 / 3, 0.0f,
		 0.0f,     1.0f * std::numbers::sqrt3 / 3, 0.0f,
		-0.5f / 2, 0.5f * std::numbers::sqrt3 / 6, 0.0f,
		 0.5f / 2, 0.5f * std::numbers::sqrt3 / 6, 0.0f,
		 0.0f    ,-0.5f * std::numbers::sqrt3 / 3, 0.0f
	};
	GLuint sirpinski_triangle_indices[] = {
		0, 3, 5,
		3, 2, 4,
		5, 4, 1
	};

	// create the shaders themselves inside openGL
	
	lgm::Shader vertexShader(GL_VERTEX_SHADER, "shaders/default_vertex.glsl");
	lgm::Shader fragmentShader(GL_FRAGMENT_SHADER, "shaders/default_fragment.glsl");

	// link shaders into one shader program

	lgm::ShaderProgram shaderProgram; 
	shaderProgram << vertexShader.get() << fragmentShader.get();
	shaderProgram.link();

	// pack a vertex buffer object which will be sent to the GPU
	
	lgm::VAO VAO;
	lgm::VBO VBO({sirpinski_triangle_vertices});
	lgm::IBO IBO({sirpinski_triangle_indices});

	VBO.addAttrib({ 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0 });
	
	VAO.linkBuffer(VBO);
	VAO.linkBuffer(IBO);

	// main loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		// clear window to a color
		glClearColor(0.08f, 0.55f, 0.63f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// run shader program on GPU
		shaderProgram.use();
		VAO.bind();
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

		// swap buffers
		glfwSwapBuffers(window);
	}

	// once done, kill all windows we opened
	glfwDestroyWindow(window);
	// terminate GLFW
	glfwTerminate();


	return 0;
}