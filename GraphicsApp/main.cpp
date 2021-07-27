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
	lgm::Window window(lgm::vector2i{ 1000, 600 }, "Lit Graphics!");
	window.setBackgroundColor({0.08f, 0.55f, 0.63f, 1.0f});

	//weird shape
	lgm::Polygon TestShape;
	TestShape.setColor({ 0.47f, 0.85f, 0.74f, 1.0f });
	TestShape.pushVertex({ -60, 0 })
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
	TestShape.triangulate();
	TestShape.transform.scale = { 0.3f, 0.3f };
	//TestShape.transform.position = { 150, 200 };
	//TestShape.transform.angle = -std::numbers::pi / 3.f;

	// main loop
	while (window.isOpen()) {
		glfwPollEvents();

		// Game Logic
		if (window.isPressed(lgm::Key::A)) TestShape.transform.position.x -= 250 * window.getDeltaTime();
		if (window.isPressed(lgm::Key::D)) TestShape.transform.position.x += 250 * window.getDeltaTime();
		if (window.isPressed(lgm::Key::W)) TestShape.transform.position.y += 250 * window.getDeltaTime();
		if (window.isPressed(lgm::Key::S)) TestShape.transform.position.y -= 250 * window.getDeltaTime();

		if (window.isPressed(lgm::Key::Q)) TestShape.transform.angle += 2 * std::numbers::pi * window.getDeltaTime();
		if (window.isPressed(lgm::Key::E)) TestShape.transform.angle -= 2 * std::numbers::pi * window.getDeltaTime();

		// Draw Shapes!
		window.draw(TestShape, true);

		// swap buffers
		window.display();

		//std::cout << "fps: " << 1.f / window.getDeltaTime() << '\n';
	}

	return 0;
}