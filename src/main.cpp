#include <iostream>
#include <fstream>
#include <cstdlib>

#include "ir/graph.hpp"
#include "third_party/OnnxImporter.hpp"
#include "viz/GraphvizDump.hpp"


int main(int argc, const char** argv) {
    
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input.onnx> <output.dot>\n";
        return EXIT_FAILURE;
    }

    std::string onnx_file = argv[1];
    std::string dot_file  = argv[2];

    ir::Graph graph;
    ir::ONNXImporter importer;

    
    if (!importer.fileImport(onnx_file, graph)) {
        std::cerr << "Failed to import ONNX model\n";
        return EXIT_FAILURE;
    }

    {
        std::string error;
        if (!graph.CheckInvariants(&error)) {
            std::cerr << "Graph invariant check failed: " << error << "\n";
        }
    }

    std::ofstream ofs(dot_file);
    if (!ofs.is_open()) {
        std::cerr << "Failed to open output file: " << dot_file << "\n";
        return EXIT_FAILURE;
    }

    graphviz::Dump dumper;
    dumper(graph, ofs);
    ofs.close();

    std::cout << "Successfully converted " << onnx_file << " to " << dot_file << "\n";
    return EXIT_SUCCESS;
}