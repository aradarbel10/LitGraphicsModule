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
	window.setBackgroundColor({0.08f, 0.55f, 0.63f, 1.0f});

	//weird shape
	lgm::Polygon triangulationTest;
	triangulationTest.setColor({ 0.47f, 0.85f, 0.74f, 1.0f });
	//triangulationTest.transform.scale = { 600, 600 };
	/*triangulationTest.pushVertex({ -0.2f, 0.0f })
		.pushVertex({ 0.0f, 0.9f })
		.pushVertex({ 0.0f, 0.0f })
		.pushVertex({ 0.6f, 0.0f })
		.pushVertex({ 0.45f, 0.5f })
		.pushVertex({ 0.25f, 0.4f })
		.pushVertex({ 0.4f, 0.8f })
		.pushVertex({ 0.9f, 0.2f })
		.pushVertex({ 0.5f, -0.5f })
		.pushVertex({ 0.0f, -0.7f })
		.pushVertex({ -0.5f, -0.5f });*/

	triangulationTest.pushVertex({ -60, 0 })
		.pushVertex({ 0.f, 270.f })
		.pushVertex({ 0.f, 0.f })
		.pushVertex({ 180.f, 0.f })
		.pushVertex({ 135.f, 150.f })
		.pushVertex({ 72.f, 120.f })
		.pushVertex({ 120.f, 240.f })
		.pushVertex({ 270.f, 60.f })
		.pushVertex({ 150.f, -150.f })
		.pushVertex({ 0.f, -210.f })
		.pushVertex({ -150.f, -150.f });
	triangulationTest.triangulate();
	triangulationTest.transform.scale = { 0.3f, 0.3f };
	triangulationTest.transform.position = { 150, 200 };
	triangulationTest.transform.angle = -std::numbers::pi / 3.f;

	// main loop
	while (window.isOpen()) {
		glfwPollEvents();

		// Draw Shapes!
		window.draw(triangulationTest);

		// swap buffers
		window.display();

		//std::cout << "fps: " << 1.f / window.getDeltaTime() << '\n';
	}

	return 0;
}