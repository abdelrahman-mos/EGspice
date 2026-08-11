#include "../../include/Circuit.hpp"
#include "../../include/Command.hpp"

void OP::report(std::shared_ptr<Circuit> circuit, std::shared_ptr<Matrix<double>> outputs) {
    std::stringstream message;
    message << std::scientific << "OP Results:\n";
    auto node_map = circuit->nodeMap();
    for (auto& curr_node : node_map) {
        int node_num = curr_node.second;
        double node_value = 0.0;
        if (node_num != 0) {
            node_value = (*outputs)[node_num-1][0];
        }
        message << "\tV( " << curr_node.first << " ) = " << to_spice_engineering(node_value) << "V\n";
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
            message << "\tI( " << terminals[2] << ", " << terminals[3] << " ) = " << to_spice_engineering(current) << "A\n";
        } else if (typeid(*curr_vsource) == typeid(CCVS)) {
            message << "\tI( " << terminals[2] << ", " << terminals[3] << " ) = " << to_spice_engineering(current) << "A\n";
            current = (*outputs)[stamp_index+1][0];
            message << "\tI( " << curr_vsource->name() << " ) = " << to_spice_engineering(current) << "A\n";
            j++;
        } else {
            message << "\tI( " << curr_vsource->name() << " ) = " << to_spice_engineering(current) << "A\n";
        }
    }

    for (int i = 0; i < num_inductors; i++) {
        auto curr_inductor = inductors[i];
        double current = (*outputs)[num_nodes+num_vsources-1+curr_inductor->inductor_id][0];
        message << "\tI( " << curr_inductor->name() << " ) = " << to_spice_engineering(current) << "A\n";
    }
    
    message << "\n";
    logger_->log(message.str());
}

void OP::stamp(std::shared_ptr<Circuit> circuit, std::shared_ptr<Matrix<double>>& coeff, std::shared_ptr<Matrix<double>>& free_term) {
    size_t num_nodes = circuit->numNodes();
    size_t num_vsources = circuit->numVsources();
    size_t num_inductors = circuit->numInductors();
    if ((coeff == nullptr) || first_point) {
        coeff = std::make_shared<Matrix<double>>(num_nodes+num_vsources+num_inductors, num_nodes+num_vsources+num_inductors);
        free_term = std::make_shared<Matrix<double>>(num_nodes+num_vsources+num_inductors, 1);
        first_point = false;
    }
    auto components = circuit->components();
    for (const auto& component : components) {
        component->stamp(coeff, free_term, num_vsources, num_inductors);
    }
}

void OP::run(std::shared_ptr<Circuit> circuit, std::shared_ptr<Matrix<double>> coeff, std::shared_ptr<Matrix<double>> free_term) {
    stamp(circuit, coeff, free_term);
    auto outputs = Matrix<double>::solve_matrix(coeff, free_term);
    auto residual = std::make_shared<Matrix<double>>((*free_term) - (*coeff) * (*outputs));
    auto errors = Matrix<double>::solve_matrix(coeff, residual);
    while (any_fail(outputs, errors)) {
        for (size_t j = 0; j < errors->numRows(); j++) {
            (*outputs)[j][0] += (*errors)[j][0];
        }
        residual = std::make_shared<Matrix<double>>((*free_term) - (*coeff) * (*outputs));
        errors = Matrix<double>::solve_matrix(coeff, residual);
    }
    report(circuit, outputs);
    return;
}