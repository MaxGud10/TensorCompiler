#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace ir
{

enum class DType : uint8_t
{
  Unknown = 0,
  Float32,
  Float16,
  Float64,
  Int64,
  Int32,
  Int8,
  UInt8,
  Bool
};

struct Dim
{
  std::optional<int64_t>     value;
  std::optional<std::string> sym;

  static Dim Unknown()
  {
    return Dim{};
  }

  static Dim Known(int64_t v)
  {
    Dim d;
    d.value = v;

    return d;
  }

  static Dim Symbolic(std::string s)
  {
    Dim d;
    d.sym = std::move(s);
    
    return d;
  }
};

struct Shape
{
  std::vector<Dim> dims;
};

} // namespace ir