#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "ir/ids.hpp"
#include "ir/node.hpp"
#include "ir/value.hpp"

namespace ir
{

class Graph
{
public:
  ValueId AddValue(std::string name,
                   ValueKind   kind  = ValueKind::Intermediate,
                   DType       dtype = DType::Unknown,
                   Shape       shape = {});


  ValueId GetOrAddValueByName(const std::string &name,
                              ValueKind default_kind = ValueKind::Intermediate);

  NodeId AddNode(std::string          op_type,
                 std::vector<ValueId> inputs,
                 std::vector<ValueId> outputs,
                 AttributeMap         attrs  = {},
                 std::string          name   = "",
                 std::string          domain = "");

  void MarkAsGraphInput (ValueId v);
  void MarkAsGraphOutput(ValueId v);

  const std::vector<Node>  &Nodes()  const { return nodes_; }
  const std::vector<Value> &Values() const { return values_; }

  std::vector<Node>  &NodesMut()  { return nodes_; }
  std::vector<Value> &ValuesMut() { return values_; }

  const Node &GetNode   (NodeId id) const;
        Node &GetNodeMut(NodeId id);

  const Value &GetValue   (ValueId id) const;
        Value &GetValueMut(ValueId id);

  ValueId FindValueByName(const std::string &name) const;

  const std::vector<ValueId> &Inputs()  const { return graph_inputs_; }
  const std::vector<ValueId> &Outputs() const { return graph_outputs_;}

  bool CheckInvariants(std::string *error = nullptr) const;

private:
  std::string GenValueName();

private:
  std::vector<Node>  nodes_;
  std::vector<Value> values_;

  std::vector<ValueId> graph_inputs_;
  std::vector<ValueId> graph_outputs_;

  std::unordered_map<std::string, ValueId> value_by_name_;

  uint32_t next_value_suffix_ = 0;
};

} // namespace ir