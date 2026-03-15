#include "gapbuffer.hpp"

class GapBufferTest : public testing::Test {
    protected:
    GapBufferTest() {
        buffer.insert('t');
    }

    GapBuffer buffer;
}