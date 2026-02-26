#pragma once


#include <string>
#include <vector>

#include "ir/ids.hpp"
#include "ir/types.hpp"

namespace ir
{

enum class ValueKind : uint8_t
{
  Input,
  Initializer,
  Intermediate,
  Output
};

struct Value
{
  ValueId id = kInvalidValue;

  std::string name;
  DType       dtype = DType::Unknown;
  Shape       shape;

  ValueKind kind = ValueKind::Intermediate;

  NodeId producer = kInvalidNode;

  std::vector<NodeId> users;
};

} // namespace ir