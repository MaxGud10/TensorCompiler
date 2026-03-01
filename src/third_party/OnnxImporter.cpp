#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

#include <onnx/onnx_pb.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/text_format.h>

#include "third_party/OnnxImporter.hpp"


namespace ir {

bool ONNXImporter::fileImport(const std::string& filename, Graph& graph) {
    
    std::ifstream ifs(filename, std::ios::binary);
    if (!ifs.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return false;
    }

    onnx::ModelProto model;
    if (!model.ParseFromIstream(&ifs)) {
        
        ifs.close();
        ifs.open(filename);
        if (!ifs.is_open()) return false;
        google::protobuf::io::IstreamInputStream input_stream(&ifs);
        if (!google::protobuf::TextFormat::Parse(&input_stream, &model)) {
            std::cerr << "Failed to parse ONNX model." << std::endl;
            return false;
        }
        
    }


    const auto& graph_proto = model.graph();
    std::unordered_map<std::string, ValueId> name_to_id;
    std::unordered_set<std::string> initializer_names;


    for (const auto& init : graph_proto.initializer()) {
        
        const std::string& name = init.name();
        initializer_names.insert(name);
        ValueId id = graph.AddValue(name, ValueKind::Initializer);
        name_to_id[name] = id;

    }


    for (const auto& input : graph_proto.input()) {
        
        const std::string& name = input.name();
        if (initializer_names.count(name)) continue;
        ValueId id = graph.AddValue(name, ValueKind::Input);
        name_to_id[name] = id;
        graph.MarkAsGraphInput(id);

    }


    for (const auto& node_proto : graph_proto.node()) {
        std::vector<ValueId> inputs;
        for (const std::string& in_name : node_proto.input()) {
            
            auto it = name_to_id.find(in_name);
            if (it == name_to_id.end()) {
                std::cerr << "Error: Input '" << in_name << "' not found for node '"
                          << node_proto.name() << "'" << std::endl;
                return false;
            }
            inputs.push_back(it->second);

        }

        std::vector<ValueId> outputs;
        for (const std::string& out_name : node_proto.output()) {
            
            if (name_to_id.count(out_name)) {
                std::cerr << "Error: Output '" << out_name << "' already exists" << std::endl;
                return false;
            }
            ValueId id = graph.AddValue(out_name, ValueKind::Intermediate);
            name_to_id[out_name] = id;
            outputs.push_back(id);
        
        }

        graph.AddNode(node_proto.op_type(), std::move(inputs), std::move(outputs),
                      {}, node_proto.name(), node_proto.domain());
    }

    
    for (const auto& output : graph_proto.output()) {
        
        auto it = name_to_id.find(output.name());
        if (it == name_to_id.end()) {
            std::cerr << "Error: Output '" << output.name() << "' not found" << std::endl;
            return false;
        }
        graph.MarkAsGraphOutput(it->second);

    }

    return true;
}

} // namespace ir