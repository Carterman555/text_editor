#include <SFML/Graphics.hpp>
#include <iostream>
#include "gapbuffer.hpp"

using namespace std;

int main() {

	GapBuffer gapBuffer;

	gapBuffer.insert('c');
	gapBuffer.insert('a');
	gapBuffer.insert('r');
	gapBuffer.insert('t');
	gapBuffer.insert('e');
	gapBuffer.insert('r');
	gapBuffer.insert('1');
	gapBuffer.insert('2');

	gapBuffer.move_gap(6);
	gapBuffer.move_gap(0);

	gapBuffer.print();

	return 0;

	sf::RenderWindow window(sf::VideoMode({ 800, 800 }), "My Window");
	window.setPosition({ 50, 50 });

	sf::Font font("CONSOLA.TTF");
	sf::Text text(font);

	text.setString("Hello");
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::White);

	while (window.isOpen()) {
		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}
			else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
				if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
					window.close();
			}
			else if (const auto* textEntered = event->getIf<sf::Event::TextEntered>()) {
				if (textEntered->unicode < 128) {
					char c = static_cast<char>(textEntered->unicode);
					cout << c << endl;
					text.setString(text.getString() + c);
				}
			}
		}

		window.clear(sf::Color(25, 25, 25));

		window.draw(text);

		window.display();
	}

}
