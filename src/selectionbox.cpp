#include <iostream>
#include "selectionbox.hpp"
#include "constants.hpp"

using namespace std;
using namespace Constants;

SelectionBox::SelectionBox(const sf::Text& text) : text(text) {
    shape.setSize(sf::Vector2f(0, FONT_SIZE));
    shape.setFillColor(sf::Color(103, 190, 217, 100));
}

void SelectionBox::create(int start, int end) {
    this->start = start;
    this->end = end;
    
    int size = abs(start - end);
    int first = min(start, end);

    shape.setPosition(text.findCharacterPos(first) + TEXT_SHAPE_OFFSET);
    shape.setSize(sf::Vector2f(size * CHARACTER_WIDTH, FONT_SIZE));
}

void SelectionBox::move_end(int pos) {
    end = pos;
}

void SelectionBox::clear() {

}
