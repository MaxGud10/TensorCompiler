#include <unordered_map>
#include "viz/GraphvizDump.hpp"


namespace graphviz {

static std::string quotation(const std::string& s) {
    return "\"" + s + "\"";
}

void Dump::operator()(const ir::Graph& graph, std::ostream& os) const {
    
    os << "digraph DF {\n";
    os << "    bgcolor=\"#FAECFF\"\n";
    os << "    node [color=\"#1B9E97\", margin=0.3, style=\"filled, rounded\", fillcolor=white, penwidth=1.9, shape=rect, fontname=\"Courier\"]\n";
    os << "    edge [arrowhead=normal, penwidth=0.5, arrowsize=0.5, color=black]\n\n";

    std::unordered_map<ir::ValueId, std::string> valueNodeIds;


    for (ir::ValueId vid : graph.Inputs()) {
        const ir::Value& val = graph.GetValue(vid);
        std::string nodeId = "input_" + std::to_string(vid);
        valueNodeIds[vid] = nodeId;
        os << "    " << nodeId << " [shape=ellipse, style=\"filled, rounded\", fillcolor=\"#1B9E97\", fontcolor=white, label=" << quotation(val.name) << "];\n";
    }


    for (ir::ValueId vid : graph.Outputs()) {
        const ir::Value& val = graph.GetValue(vid);
        std::string nodeId = "output_" + std::to_string(vid);
        valueNodeIds[vid] = nodeId;
        os << "    " << nodeId << " [shape=ellipse, style=\"filled, rounded\", fillcolor=\"#1B9E97\", fontcolor=white, label=" << quotation(val.name) << "];\n";
    }


    for (const ir::Value& val : graph.Values()) {
        if (val.kind == ir::ValueKind::Initializer) {
            std::string nodeId = "init_" + std::to_string(val.id);
            valueNodeIds[val.id] = nodeId;
            os << "    " << nodeId << " [style=\"filled, rounded\", fillcolor=lightgrey, label="
               << quotation(val.name) << "];\n";
        }
    }


    for (const ir::Node& node : graph.Nodes()) {
        std::string nodeId = "node_" + std::to_string(node.id);
        os << "    " << nodeId << " [label=" << quotation(node.op_type) << "];\n";
    }
    

    for (const ir::Value& val : graph.Values()) {
        
        if (val.users.empty()) continue;

        std::string srcId;
        auto it = valueNodeIds.find(val.id);
        if (it != valueNodeIds.end()) {
            srcId = it->second;
        } else if (val.producer != ir::kInvalidNode) {
            srcId = "node_" + std::to_string(val.producer);
        } else {
            continue;
        }

        for (ir::NodeId user : val.users) {
            std::string dstId = "node_" + std::to_string(user);
            os << "    " << srcId << " -> " << dstId
               << " [label=" << quotation(val.name) << "];\n";
        }

    }


    for (ir::ValueId vid : graph.Outputs()) {
        const ir::Value& val = graph.GetValue(vid);
        if (val.producer != ir::kInvalidNode) {
            auto it = valueNodeIds.find(vid);
            if (it != valueNodeIds.end()) {
                std::string srcId = "node_" + std::to_string(val.producer);
                std::string dstId = it->second;
                os << "    " << srcId << " -> " << dstId
                   << " [label=" << quotation(val.name) << "];\n";
            }
        }
    }

    os << "}\n";
}

} // namespace graphviz