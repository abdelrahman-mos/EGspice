#include "../../include/Circuit.hpp"
#include "../../include/Command.hpp"

void OP::report(std::shared_ptr<Circuit> circuit, std::shared_ptr<Matrix<double>> outputs) {
    std::string message = "OP Results:\n";
    auto node_map = circuit->nodeMap();
    for (auto& curr_node : node_map) {
        int node_num = curr_node.second;
        double node_value = 0.0;
        if (node_num != 0) {
            node_value = (*outputs)[node_num-1][0];
        }
        message += "\tV( " + curr_node.first + " ) = " + std::to_string(node_value) + "V\n";
    }

    auto vsources = circuit->vsources();
    auto inductors = circuit->inductors();
    int num_nodes = circuit->numNodes();
    int num_vsources = circuit->numVsources();
    int num_inductors = circuit->numInductors();
    for (int i = 0; i < num_vsources; i++) {
        auto curr_vsource = vsources[i];
        double current = (*outputs)[num_nodes-num_inductors+curr_vsource->id][0];
        if (typeid(*curr_vsource) == typeid(CCCS)) {
            auto terminals = curr_vsource->get_terminals();
            message += "\tI( " + terminals[2] + ", " + terminals[3] + " ) = " + std::to_string(current) + "A\n";
        } else {
            message += "\tI( " + curr_vsource->name() + " ) = " + std::to_string(current) + "A\n";
        }
    }

    for (int i = 0; i < num_inductors; i++) {
        auto curr_inductor = inductors[i];
        double current = (*outputs)[num_nodes+num_vsources-1+curr_inductor->inductor_id][0];
        message += "\tI( " + curr_inductor->name() + " ) = " + std::to_string(current) + "A\n";
    }

    logger_->log(message + "\n");
}

void OP::run(std::shared_ptr<Circuit> circuit) {
    circuit->stamp_circuit();
    auto circuit_matrix = circuit->get_matrix();
    auto output_matrix = circuit->get_output_matrix();
    auto outputs = Matrix<double>::solve_matrix(circuit_matrix, output_matrix);
    report(circuit, outputs);
    return;
}