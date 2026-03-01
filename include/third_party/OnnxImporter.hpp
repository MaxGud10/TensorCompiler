#pragma once

#include <string>

#include "ir/graph.hpp"


namespace ir {

class ONNXImporter {
public:
    bool fileImport (const std::string& filename, Graph& graph);
};

} 