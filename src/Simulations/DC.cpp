#include "../../include/Circuit.hpp"
#include "../../include/Command.hpp"

void DC::report(std::shared_ptr<Circuit> circuit, std::shared_ptr<Matrix<double>> outputs) {
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
    for (int i = 0, j=0; i < num_vsources; i++) {
        auto curr_vsource = vsources[i-j];
        int stamp_index = num_nodes-num_inductors+curr_vsource->id;
        double current = (*outputs)[stamp_index][0];
        auto terminals = curr_vsource->get_terminals();
        if (typeid(*curr_vsource) == typeid(CCCS)) {
            message += "\tI( " + terminals[2] + ", " + terminals[3] + " ) = " + std::to_string(current) + "A\n";
        } else if (typeid(*curr_vsource) == typeid(CCVS)) {
            message += "\tI( " + terminals[2] + ", " + terminals[3] + " ) = " + std::to_string(current) + "A\n";
            current = (*outputs)[stamp_index+1][0];
            message += "\tI( " + curr_vsource->name() + " ) = " + std::to_string(current) + "A\n";
            j++;
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

void DC::expand_points() {
    for (double curr_voltage = start_outer; curr_voltage < end_outer; curr_voltage+=step_outer) {
        outer_points.push_back(curr_voltage);
    }

    if (inner_vsource_id == -1) return;
    for (double curr_voltage = start_inner; curr_voltage < end_inner; curr_voltage+=step_inner) {
        inner_points.push_back(curr_voltage);
    }
}

void DC::stamp(std::shared_ptr<Circuit> circuit, std::shared_ptr<Matrix<double>>& coeff, std::shared_ptr<Matrix<double>>& free_term, double outer_value, double inner_value) {
    size_t num_vsources = circuit->numVsources();
    size_t num_inductors = circuit->numInductors();
    if ((coeff == nullptr) || first_point) {
        size_t num_nodes = circuit->numNodes();
        coeff = std::make_shared<Matrix<double>>(num_nodes+num_vsources+num_inductors, num_nodes+num_vsources+num_inductors);
        free_term = std::make_shared<Matrix<double>>(num_nodes+num_vsources+num_inductors, 1);
        for (const auto& component : circuit->components()) {
            component->stamp(coeff, free_term, num_vsources, num_inductors);
        }
        first_point = false;
    } else {
        for (const auto& curr_vsource : circuit->vsources()) {
            // in repitition, only stamp changing voltages
            if ((curr_vsource->id == outer_vsource_id) || (curr_vsource->id == inner_vsource_id)) {
                curr_vsource->stamp(coeff, free_term, num_vsources, num_inductors);
            }
        }
    }
}

void DC::run(std::shared_ptr<Circuit> circuit, std::shared_ptr<Matrix<double>> coeff, std::shared_ptr<Matrix<double>> free_term) {
    stamp(circuit, coeff, free_term, 0.0, 0.0);
    auto outputs = Matrix<double>::solve_matrix(coeff, free_term);
    report(circuit, outputs);
    return;
}