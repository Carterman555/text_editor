#include <gtest/gtest.h>
#include "gapbuffer.hpp"


TEST(BufferTest, BasicInsert) {

  GapBuffer buffer(5);

  buffer.insert('a', 0);
  EXPECT_EQ(buffer.get_str(), "a____");

  buffer.insert('p', 1);
  EXPECT_EQ(buffer.get_str(), "ap___");

  buffer.insert('p', 2);
  buffer.insert('l', 3);
  buffer.insert('e', 4);

  EXPECT_EQ(buffer.get_str(), "apple");

  buffer.insert('s', 5);

  EXPECT_EQ(buffer.get_str(), "apples____");
}

TEST(BufferTest, BasicRemove) {

  GapBuffer buffer(5);


}

