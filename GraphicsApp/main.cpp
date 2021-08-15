#include <iostream>
#include <numbers>
#include <string>

#include "../LitGraphics/Window.h"

int main() {
	// opening a window using GLFW
	lgm::Window window(lgm::vector2i{ 1000, 600 }, "Lit Graphics!");
	window.setBackgroundColor({ 0.08f, 0.55f, 0.63f, 1.0f });

	// test texture -- brick wall
	lgm::Texture myTexture("C:/Users/arada/Pictures/brick wall.jpg");

	// weird shape
	lgm::Polygon TestShape({
		{ -60, 0 },
		{ 0.f, 270.f },
		{ 0.f, 0.f },
		{ 180.f, 0.f },
		{ 135.f, 150.f },
		{ 72.f, 120.f },
		{ 120.f, 240.f },
		{ 270.f, 60.f },
		{ 150.f, -150.f },
		{ 0.f, -210.f },
		{ -150.f, -150.f }
		});
	TestShape.setColor({ 0.47f, 0.85f, 0.74f, 1.0f });
	TestShape.transform.scale = { 0.3f, 0.3f };
	TestShape.texture_transform.scale = { 0.001f, 0.001f };
	TestShape.setTexture(myTexture);
	
	// donut
	lgm::Polygon Donut({ {0, 3}, {3, 3}, {3, -3}, {-3, -3}, {-3, 3}, {-0, 3},
		{-0, 2}, {-2, 2}, {-2, -2}, {2, -2}, {2, 2}, {0, 2} });
	Donut.setColor({ 0.83f, 0.51f, 0.96f, 0.4f });
	Donut.transform.scale = { 8.f, 8.f };
	Donut.texture_transform.scale = { 0.1f, 0.1f };

	// main loop
	while (window.isOpen()) {
		window.pollEvents();

		// Game Logic
		if (window.isPressed(lgm::Key::A)) TestShape.transform.position.x -= 250 * window.getDeltaTime();
		if (window.isPressed(lgm::Key::D)) TestShape.transform.position.x += 250 * window.getDeltaTime();
		if (window.isPressed(lgm::Key::W)) TestShape.transform.position.y += 250 * window.getDeltaTime();
		if (window.isPressed(lgm::Key::S)) TestShape.transform.position.y -= 250 * window.getDeltaTime();

		if (window.isPressed(lgm::Key::Q)) TestShape.transform.angle += 2 * std::numbers::pi * window.getDeltaTime();
		if (window.isPressed(lgm::Key::E)) TestShape.transform.angle -= 2 * std::numbers::pi * window.getDeltaTime();


		Donut.transform.position = { window.getMousePos().x - (window.getSize().x / 2), -window.getMousePos().y + (window.getSize().y / 2) };
		Donut.transform.angle += (float)window.getMouseScroll().y * 50.f * std::numbers::pi * window.getDeltaTime();

		if (window.isMousePressed(lgm::Mouse::Left) || window.isMousePressed(lgm::Mouse::Right)) Donut.transform.scale = { 18.f, 18.f };
		else Donut.transform.scale = { 8.f, 8.f };


		// Draw Shapes!
		window.draw(TestShape, true);
		window.draw(Donut, true);

		// swap buffers
		window.display();
	}

	return 0;
}