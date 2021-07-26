#include <iostream>
#include <numbers>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

import Shader;
import VertexArray;
import Window;
import Tuple;
import Polygon;

int main() {
	// opening a window using GLFW
	lgm::Window window(lgm::vector2i{ 600, 600 }, "Lit Graphics!");
	//window.setBackgroundColor({0.08f, 0.55f, 0.63f, 1.0f});

	// constructing polygons
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
		-0.5f,    -0.5f * std::numbers::sqrt3 / 3, 1.00f, 0.97f, 0.45f,
		 0.5f,    -0.5f * std::numbers::sqrt3 / 3, 0.47f, 0.85f, 0.74f,
		 0.0f,     1.0f * std::numbers::sqrt3 / 3, 0.96f, 0.52f, 0.94f,
		-0.5f / 2, 0.5f * std::numbers::sqrt3 / 6, 1.0f, 1.0f, 1.0f,
		 0.5f / 2, 0.5f * std::numbers::sqrt3 / 6, 1.0f, 1.0f, 1.0f,
		 0.0f    ,-0.5f * std::numbers::sqrt3 / 3, 1.0f, 1.0f, 1.0f
	};
	GLuint sirpinski_triangle_indices[] = {
		0, 3, 5,
		3, 2, 4,
		5, 4, 1
	};

	//pentagon construction
	lgm::Polygon pentagon;
	pentagon.pushVertex({ 0.309, 0.9511 })
		.pushVertex({ 1, 0 })
		.pushVertex({ 0.309, -0.9511 })
		.pushVertex({ -0.809, -0.5878 })
		.pushVertex({ -0.809, 0.5878 })
		.triangulate();

	//weird shape
	lgm::Polygon triangulationTest;
	triangulationTest.setColor({ 0.47f, 0.85f, 0.74f, 1.0f });
	triangulationTest.pushVertex({ -0.2f, 0.0f })
		.pushVertex({ 0.0f, 0.9f })
		.pushVertex({ 0.0f, 0.0f })
		.pushVertex({ 0.6f, 0.0f })
		.pushVertex({ 0.45f, 0.5f })
		.pushVertex({ 0.25f, 0.4f })
		.pushVertex({ 0.4f, 0.8f })
		.pushVertex({ 0.9f, 0.2f })
		.pushVertex({ 0.5f, -0.5f })
		.pushVertex({ 0.0f, -0.7f })
		.pushVertex({ -0.5f, -0.5f })
		.triangulate();

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

	VBO.addAttrib({ 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0 });
	VBO.addAttrib({ 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)) });
	
	VAO.linkBuffer(VBO);
	VAO.linkBuffer(IBO);

	// main loop
	while (window.isOpen()) {
		glfwPollEvents();

		// Draw Shapes!
		triangulationTest.draw(shaderProgram, true);

		// swap buffers
		window.display();
	}

	return 0;
}