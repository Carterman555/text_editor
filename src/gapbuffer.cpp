#include <iostream>
#include "gapbuffer.hpp"

GapBuffer::GapBuffer() {
    std::cout << "Gap buffer initialized" << std::endl;
}

void GapBuffer::insert(char c) {
    std::cout << "inserting " << c << std::endl;
}