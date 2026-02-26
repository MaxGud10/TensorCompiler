#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace ir
{
using Attribute = std::variant<
                  int64_t,
                  double,
                  std::string,
                  std::vector<int64_t>,
                  std::vector<double>,
                  std::vector<std::string>>;

using AttributeMap = std::unordered_map<std::string, Attribute>;

template <class T>
inline const T *GetAttrIf(const AttributeMap &m, const std::string &key)
{
  auto it = m.find(key);
  if (it == m.end())
    return nullptr;

  return std::get_if<T>(&it->second);
}

} // namespace ir