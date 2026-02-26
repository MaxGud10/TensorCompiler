#pragma once

#include <string>
#include <vector>

#include "ir/attribute.hpp"
#include "ir/ids.hpp"

namespace ir
{

struct Node
{
  NodeId id = kInvalidNode;

  std::string op_type;
  std::string name;
  std::string domain;

  std::vector<ValueId> inputs;
  std::vector<ValueId> outputs;

  AttributeMap attrs;
};

} // namespace ir