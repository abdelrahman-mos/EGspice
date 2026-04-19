#include "../../include/Circuit.hpp"
#include "../../include/Command.hpp"

void DC::report_curr_idx(std::shared_ptr<Circuit> circuit, std::shared_ptr<Matrix<double>> outputs, std::stringstream& message, size_t curr_idx,
    std::string pre_text) {
    auto node_map = circuit->nodeMap();
    for (auto& curr_node : node_map) {
        int node_num = curr_node.second;
        double node_value = 0.0;
        if (node_num != 0) {
            node_value = (*outputs)[curr_idx][node_num-1];
        }
        message << pre_text << "\tV( " << curr_node.first << " ) = " << to_spice_engineering(node_value) << "V\n";
    }

    auto vsources = circuit->vsources();
    auto inductors = circuit->inductors();
    int num_nodes = circuit->numNodes();
    int num_vsources = circuit->numVsources();
    int num_inductors = circuit->numInductors();
    for (int i = 0, j=0; i < num_vsources; i++) {
        auto curr_vsource = vsources[i-j];
        int stamp_index = num_nodes-num_inductors+curr_vsource->id;
        double current = (*outputs)[curr_idx][stamp_index];
        auto terminals = curr_vsource->get_terminals();
        if (typeid(*curr_vsource) == typeid(CCCS)) {
            message << pre_text << "\tI( " << terminals[2] << ", " << terminals[3] << " ) = " << to_spice_engineering(current) << "A\n";
        } else if (typeid(*curr_vsource) == typeid(CCVS)) {
            message << pre_text << "\tI( " << terminals[2] << ", " << terminals[3] << " ) = " << to_spice_engineering(current) << "A\n";
            current = (*outputs)[curr_idx][stamp_index+1];
            message << pre_text << "\tI( " << curr_vsource->name() << " ) = " << to_spice_engineering(current) + "A\n";
            j++;
        } else {
            message << pre_text << "\tI( " << curr_vsource->name() << " ) = " << to_spice_engineering(current) + "A\n";
        }
    }

    for (int i = 0; i < num_inductors; i++) {
        auto curr_inductor = inductors[i];
        double current = (*outputs)[curr_idx][num_nodes+num_vsources-1+curr_inductor->inductor_id];
        message << pre_text << "\tI( " << curr_inductor->name() << " ) = " << to_spice_engineering(current) + "A\n";
    }
}

void DC::report(std::shared_ptr<Circuit> circuit, std::shared_ptr<Matrix<double>> outputs) {
    std::stringstream message;
    message << "DC Results:\n";
    size_t curr_idx = 0;
    for (size_t curr_point_outer = 0; curr_point_outer < outer_points.size(); curr_point_outer++) {
        message << outer_vsource_name << " = " << to_spice_engineering(outer_points[curr_point_outer]) << "V\n";
        if (inner_vsource_name != "") {
            for (size_t curr_point_inner = 0; curr_point_inner < inner_points.size(); curr_point_inner++) {
                message << "\t" << inner_vsource_name << " = " << to_spice_engineering(inner_points[curr_point_inner]) << "V\n";
                report_curr_idx(circuit, outputs, message, curr_idx, "\t");
                curr_idx++;
            }
        } else {
            report_curr_idx(circuit, outputs, message, curr_idx);
            curr_idx++;
        }
    }
    
    message << "\n";
    logger_->log(message.str());
}

void DC::expand_points() {
    // step will always be entered as an absolute value
    double start_voltage = (start_outer < end_outer) ? start_outer : end_outer;
    double end_voltage = start_outer + end_outer - start_voltage; // just a mathematical funny way of if conditions
    for (double curr_voltage = start_voltage; curr_voltage <= end_voltage; curr_voltage+=step_outer) {
        outer_points.push_back(curr_voltage);
    }

    if (inner_vsource_name == "") return;
    start_voltage = (start_inner < end_inner) ? start_inner : end_inner;
    end_voltage = start_inner + end_inner - start_voltage;
    for (double curr_voltage = start_voltage; curr_voltage <= end_voltage; curr_voltage+=step_inner) {
        inner_points.push_back(curr_voltage);
    }
}

void DC::stamp(std::shared_ptr<Circuit> circuit, std::shared_ptr<Matrix<double>>& coeff, std::shared_ptr<Matrix<double>>& free_term, 
    double outer_value, double prev_outer_value, double inner_value, double prev_inner_value) {
    size_t num_vsources = circuit->numVsources();
    size_t num_inductors = circuit->numInductors();
    if ((coeff == nullptr) || first_point) {
        size_t num_nodes = circuit->numNodes();
        coeff = std::make_shared<Matrix<double>>(num_nodes+num_vsources+num_inductors, num_nodes+num_vsources+num_inductors);
        free_term = std::make_shared<Matrix<double>>(num_nodes+num_vsources+num_inductors, 1);
        for (const auto& component : circuit->components()) {
            component->stamp(coeff, free_term, num_vsources, num_inductors);
            if (auto curr_vsource = std::dynamic_pointer_cast<Vsource>(component)) {
                // delete the vsource value after stamping it
                if (curr_vsource->name() == outer_vsource_name) {
                    curr_vsource->stamp(coeff, free_term, outer_value, curr_vsource->get_value(), num_vsources, num_inductors);
                } else if (curr_vsource->name() == inner_vsource_name) {
                    curr_vsource->stamp(coeff, free_term, inner_value, curr_vsource->get_value(), num_vsources, num_inductors);
                } 
            }
        }
        first_point = false;
    } else {
        for (const auto& curr_vsource : circuit->vsources()) {
            // in repitition, only stamp changing voltages
            if (curr_vsource->name() == outer_vsource_name) {
                curr_vsource->stamp(coeff, free_term, outer_value, prev_outer_value, num_vsources, num_inductors);
            } else if ((inner_vsource_name != "") && (curr_vsource->name() == inner_vsource_name)) {
                curr_vsource->stamp(coeff, free_term, inner_value, prev_inner_value, num_vsources, num_inductors);
            }
        }
    }
}

void DC::run(std::shared_ptr<Circuit> circuit, std::shared_ptr<Matrix<double>> coeff, std::shared_ptr<Matrix<double>> free_term) {
    bool found_outer = false;
    bool found_inner = false;
    for (const auto& curr_vsource : circuit->vsources()) {
        if (curr_vsource->name() == outer_vsource_name) found_outer = true;
        if (curr_vsource->name() == inner_vsource_name) found_inner = true;
        if ((found_outer && found_inner) || (found_outer && !found_inner && (inner_vsource_name == ""))) break;
    }
    if (!found_outer) {
        logger_->log(LogLevel::ERROR, "Device " + outer_vsource_name + " not found");
        return;
    }
    if ((!found_inner) && (inner_vsource_name != "")) {
        logger_->log(LogLevel::ERROR, "Device " + inner_vsource_name + " not found");
        return;
    }
    stamp(circuit, coeff, free_term, 0.0, 0.0, 0.0, 0.0);
    size_t num_rows = coeff->numRows();
    size_t outer_num_points = outer_points.size();
    size_t inner_num_points = inner_points.size();
    size_t num_points = outer_num_points;
    if (inner_num_points) num_points *= inner_num_points;
    auto outputs_mat = std::make_shared<Matrix<double>>(num_points, num_rows);
    double prev_outer_val = 0.0;
    double prev_inner_val = 0.0;
    size_t curr_idx = 0;
    for (size_t i = 0; i < outer_num_points; i++) {
        double curr_outer_voltage = outer_points[i];
        if (inner_vsource_name != "") {
            for (size_t j = 0; j < inner_num_points; j++) {
                double curr_inner_voltage = inner_points[j];
                stamp(circuit, coeff, free_term, curr_outer_voltage, prev_outer_val, curr_inner_voltage, prev_inner_val);
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
                outputs_mat->emplace_at(outputs->transpose()[0], curr_idx++);
                prev_inner_val = curr_inner_voltage;
                prev_outer_val = curr_outer_voltage;
            }
        } else {
            stamp(circuit, coeff, free_term, curr_outer_voltage, prev_outer_val, 0.0, prev_inner_val);
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
            outputs_mat->emplace_at(outputs->transpose()[0], i);
        }
        prev_outer_val = curr_outer_voltage;
    }
    report(circuit, outputs_mat);
    return;
}