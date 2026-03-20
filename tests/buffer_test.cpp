#include <gtest/gtest.h>
#include "gapbuffer.hpp"


TEST(BufferTest, BasicInsert) {

  GapBuffer buffer(5);

  buffer.insert('a');
  EXPECT_EQ(buffer.get_str(), "a____");

  buffer.insert('p');
  EXPECT_EQ(buffer.get_str(), "ap___");

  buffer.insert('p');
  buffer.insert('l');
  buffer.insert('e');

  EXPECT_EQ(buffer.get_str(), "apple");

  buffer.insert('s');

  EXPECT_EQ(buffer.get_str(), "apples____");
}

TEST(BufferTest, MoveGapLeft) {

  GapBuffer buffer(5);

  buffer.insert('a');
  buffer.move_gap(0);
  EXPECT_EQ(buffer.get_str(), "____a");

  buffer.insert('b');
  EXPECT_EQ(buffer.get_str(), "b___a");

  buffer.insert('c');
  buffer.move_gap(1);
  EXPECT_EQ(buffer.get_str(), "b__ca");
}