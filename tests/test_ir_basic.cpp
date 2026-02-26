#include "ir/graph.hpp"

#include <gtest/gtest.h>

TEST(IR, BasicWiring)
{
  ir::Graph g;

  auto a = g.AddValue("A", ir::ValueKind::Input);
  auto b = g.AddValue("B", ir::ValueKind::Input);
  auto c = g.AddValue("C", ir::ValueKind::Intermediate);

  g.MarkAsGraphInput(a);
  g.MarkAsGraphInput(b);

  ir::AttributeMap attrs;
  attrs["broadcast"] = int64_t(0);

  auto add = g.AddNode("Add",
                       {a, b},
                       {c},
                       std::move(attrs));

  g.MarkAsGraphOutput(c);

  EXPECT_EQ(g.Nodes().size(),  1u);
  EXPECT_EQ(g.Values().size(), 3u);

  EXPECT_EQ(g.GetValue(c).producer, add);
  EXPECT_EQ(g.GetValue(a).users.size(), 1u);

  std::string err;
  EXPECT_TRUE(g.CheckInvariants(&err)) << err;
}