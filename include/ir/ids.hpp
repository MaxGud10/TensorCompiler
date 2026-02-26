#pragma once

#include <cstdint>

namespace ir
{
using NodeId  = uint32_t;
using ValueId = uint32_t;

static constexpr NodeId  kInvalidNode  = static_cast<NodeId> (-1);
static constexpr ValueId kInvalidValue = static_cast<ValueId>(-1);

} // namespace ir