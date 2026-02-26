#include "ir/graph.hpp"
#include <iostream>

int main()
{
  ir::Graph g;

  auto a = g.AddValue("A", ir::ValueKind::Input);
  auto b = g.AddValue("B", ir::ValueKind::Input);
  auto c = g.AddValue("C", ir::ValueKind::Intermediate);

  g.MarkAsGraphInput(a);
  g.MarkAsGraphInput(b);

  g.AddNode("Add", {a, b}, {c});
  g.MarkAsGraphOutput(c);

  std::cout << "nodes=" << g.Nodes().size() << " values=" << g.Values().size() << "\n";
  return 0;
}