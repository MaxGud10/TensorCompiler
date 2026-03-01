#include "ir/graph.hpp"

#include <algorithm>
#include <sstream>
#include <stdexcept>

namespace ir
{

static void AddUnique(std::vector<ValueId> &vec, ValueId v)
{
  if (std::find(vec.begin(), vec.end(), v) == vec.end())
    vec.push_back(v);
}

ValueId Graph::AddValue(std::string name,
                        ValueKind   kind,
                        DType       dtype,
                        Shape       shape)
{
  if (name.empty())
    name = GenValueName();

  auto it = value_by_name_.find(name);
  if (it != value_by_name_.end())
    return it->second;

  Value v;
  v.id    = static_cast<ValueId>(values_.size());
  v.name  = std::move(name);
  v.kind  = kind;
  v.dtype = dtype;
  v.shape = std::move(shape);

  values_.push_back(std::move(v));
  value_by_name_[values_.back().name] = values_.back().id;

  return values_.back().id;
}

ValueId Graph::GetOrAddValueByName(const std::string &name,
                                   ValueKind          default_kind)
{
  auto it = value_by_name_.find(name);
  if (it != value_by_name_.end())
    return it->second;

  return AddValue(name, default_kind);
}

NodeId Graph::AddNode(std::string          op_type,
                      std::vector<ValueId> inputs,
                      std::vector<ValueId> outputs,
                      AttributeMap         attrs,
                      std::string          name,
                      std::string          domain)
{
  Node n;
  n.id      = static_cast<NodeId>(nodes_.size());
  n.op_type = std::move(op_type);
  n.inputs  = std::move(inputs);
  n.outputs = std::move(outputs);
  n.attrs   = std::move(attrs);
  n.name    = std::move(name);
  n.domain  = std::move(domain);

  for (ValueId in : n.inputs)
  {
    if (in == kInvalidValue)
      continue;

    if (in >= values_.size())
      throw std::runtime_error("Graph::AddNode: input ValueId out of range");

    values_[in].users.push_back(n.id);
  }

  for (ValueId out : n.outputs)
  {
    if (out == kInvalidValue)
      continue;

    if (out >= values_.size())
      throw std::runtime_error("Graph::AddNode: output ValueId out of range");

    if (values_[out].producer != kInvalidNode)
      throw std::runtime_error("Graph::AddNode: value already has a producer: " + values_[out].name);

    values_[out].producer = n.id;
  }

  nodes_.push_back(std::move(n));
  return nodes_.back().id;
}

void Graph::MarkAsGraphInput(ValueId v)
{
  if (v >= values_.size())
    throw std::runtime_error("Graph::MarkAsGraphInput: ValueId out of range");

  values_[v].kind = ValueKind::Input;
  AddUnique(graph_inputs_, v);
}

void Graph::MarkAsGraphOutput(ValueId v)
{
  if (v >= values_.size())
    throw std::runtime_error("Graph::MarkAsGraphOutput: ValueId out of range");

  values_[v].kind = ValueKind::Output;
  AddUnique(graph_outputs_, v);
}

const Node &Graph::GetNode(NodeId id) const
{
  if (id >= nodes_.size())
    throw std::out_of_range("Graph::GetNode");

  return nodes_[id];
}

Node &Graph::GetNodeMut(NodeId id)
{
  if (id >= nodes_.size())
    throw std::out_of_range("Graph::GetNodeMut");

  return nodes_[id];
}

const Value &Graph::GetValue(ValueId id) const
{
  if (id >= values_.size())
    throw std::out_of_range("Graph::GetValue");

  return values_[id];
}

Value &Graph::GetValueMut(ValueId id)
{
  if (id >= values_.size())
    throw std::out_of_range("Graph::GetValueMut");

  return values_[id];
}

ValueId Graph::FindValueByName(const std::string &name) const
{
  auto it = value_by_name_.find(name);
  if (it == value_by_name_.end())
    return kInvalidValue;

  return it->second;
}

std::string Graph::GenValueName()
{
  std::ostringstream oss;
  oss << "%v" << next_value_suffix_++;
  return oss.str();
}

bool Graph::CheckInvariants(std::string *error) const
{
  for (size_t i = 0; i < values_.size(); ++i)
  {
    if (values_[i].id != static_cast<ValueId>(i))
    {
      if (error)
        *error = "Value id mismatch at index " + std::to_string(i);
      return false;
    }
  }

  for (size_t i = 0; i < nodes_.size(); ++i)
  {
    if (nodes_[i].id != static_cast<NodeId>(i))
    {
      if (error)
        *error = "Node id mismatch at index " + std::to_string(i);
      return false;
    }
  }

  for (const auto &v : values_)
  {
    if (v.producer != kInvalidNode && v.producer >= nodes_.size())
    {
      if (error)
        *error = "Value " + v.name + " has producer out of range";
      return false;
    }

    for (NodeId u : v.users)
    {
      if (u >= nodes_.size())
      {
        if (error)
          *error = "Value " + v.name + " has user out of range";
        return false;
      }
    }
  }

  for (const auto &n : nodes_)
  {
    for (ValueId in : n.inputs)
    {
      if (in >= values_.size())
      {
        if (error)
          *error = "Node input out of range: op=" + n.op_type;
        return false;
      }
    }

    for (ValueId out : n.outputs)
    {
      if (out >= values_.size())
      {
        if (error)
          *error = "Node output out of range: op=" + n.op_type;
        return false;
      }

      if (values_[out].producer != n.id)
      {
        if (error)
          *error = "Producer mismatch for value " + values_[out].name;
        return false;
      }
    }
  }

  return true;
}

} // namespace ir