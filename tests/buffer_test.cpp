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

TEST(BufferTest, MoveGap) {

  GapBuffer buffer(5);

  buffer.insert('a', 1);
  buffer.move_gap(0);
  EXPECT_EQ(buffer.get_str(), "____a");

  buffer.insert('b', 2);
  EXPECT_EQ(buffer.get_str(), "b___a");

  buffer.insert('c', 3);
  buffer.move_gap(1);
  EXPECT_EQ(buffer.get_str(), "b__ca");

  buffer.move_gap(2);
  EXPECT_EQ(buffer.get_str(), "bc__a");

  buffer.move_gap(2);
  EXPECT_EQ(buffer.get_str(), "bc__a");

  buffer.move_gap(3);
  EXPECT_EQ(buffer.get_str(), "bca__");

  buffer.move_gap(0);
  EXPECT_EQ(buffer.get_str(), "__bca");

  buffer.move_gap(3);
  EXPECT_EQ(buffer.get_str(), "bca__");
}

