#pragma once

#include <ostream>
#include "ir/graph.hpp"

namespace graphviz {

class Dump {
public:
    void operator()(const ir::Graph& graph, std::ostream& os) const;
};

} // namespace graphviz