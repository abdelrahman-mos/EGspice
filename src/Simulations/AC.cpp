#include "../../include/Command.hpp"
#include "../../include/Circuit.hpp"

void AC::expand_freq_dec() {
    double log_start = std::log10(fstart);
    double log_end = std::log10(fend);
    size_t total_points = (size_t)((log_end - log_start) * numpoints);
    
    double step = (log_end - log_start) / total_points;
    double exponent = log_start;
    frequency_points.push_back(fstart);
    for (size_t i = 1; i < total_points; i++) {
        exponent += step;
        frequency_points.push_back(std::pow(10.0, exponent));
    }
    frequency_points.push_back(fend);
}

void AC::expand_freq_oct() {
    double log_start = std::log2(fstart);
    double log_end = std::log2(fend);
    size_t total_points = (size_t)((log_end - log_start) * numpoints);
    
    double step = 1.0 / numpoints;
    double exponent = log_start;
    for (size_t i = 0; i < total_points; i++) {
        exponent += step;
        frequency_points.push_back(std::pow(2.0, exponent));
    }
}

void AC::expand_freq_lin() {
    double step = (fend - fstart) / numpoints;
    double freq = fstart;
    for (size_t i = 0; i < numpoints; i++) {
        frequency_points.push_back(freq);
        freq += step;
    }
}

void AC::expand_freq() {
    switch (type)
    {
    case AC_TYPE::DEC:
        expand_freq_dec();
        break;

    case AC_TYPE::OCT:
        expand_freq_oct();
        break;

    case AC_TYPE::LIN:
        expand_freq_lin();
        break;
    
    default:
        break;
    }
}

void AC::report(std::shared_ptr<Circuit> circuit, std::shared_ptr<Matrix<std::complex<double>>> outputs) {
    std::string message = "AC Results:\n";
    auto node_map = circuit->nodeMap();
    size_t num_points = frequency_points.size();
    for (size_t curr_point = 0; curr_point < num_points; curr_point++) {
        message += "\nFREQ: " + std::to_string(frequency_points[curr_point]) + "\n";
        for (auto& curr_node : node_map) {
            int node_num = curr_node.second;
            std::complex<double> node_value = 0.0;
            if (node_num != 0) {
                node_value = (*outputs)[curr_point][node_num-1];
            }
            message += "\tABS(V( " + curr_node.first + " )) = " + std::to_string(std::abs(node_value)) + "V";
            message += "\tPHASE(V( " + curr_node.first + " )) = " + std::to_string(std::arg(node_value)) + "rad\n";
        }

        auto vsources = circuit->vsources();
        auto inductors = circuit->inductors();
        int num_nodes = circuit->numNodes();
        int num_vsources = circuit->numVsources();
        int num_inductors = circuit->numInductors();
        for (int i = 0; i < num_vsources; i++) {
            auto curr_vsource = vsources[i];
            auto current = (*outputs)[curr_point][num_nodes-num_inductors+curr_vsource->vsource_id];
            message += "\tABS(I( " + curr_vsource->name() + " )) = " + std::to_string(std::abs(current)) + "A";
            message += "\tPHASE(I( " + curr_vsource->name() + " )) = " + std::to_string(std::arg(current)) + "rad\n";
        }

        for (int i = 0; i < num_inductors; i++) {
            auto curr_inductor = inductors[i];
            auto current = (*outputs)[num_nodes+num_vsources-1+curr_inductor->inductor_id][0];
            message += "\tABS(I( " + curr_inductor->name() + " )) = " + std::to_string(std::abs(current)) + "A";
            message += "\tPHASE(I( " + curr_inductor->name() + " )) = " + std::to_string(std::arg(current)) + "rad\n";
        }
    }
    logger_->log(message + "\n");
}

void AC::run(std::shared_ptr<Circuit> circuit) {
    circuit->stamp_circuit(1.0);
    auto circuit_matrix_ac = circuit->get_ac_matrix();
    size_t num_rows = circuit_matrix_ac->numRows();
    size_t num_points = frequency_points.size();
    auto outputs_mat = std::make_shared<Matrix<std::complex<double>>>(num_points, num_rows);
    for (size_t i = 0; i < num_points; i++) {
        double freq = frequency_points[i];
        circuit->stamp_circuit(freq);
        auto circuit_matrix_ac = circuit->get_ac_matrix();
        auto output_matrix_ac = circuit->get_ac_output_matrix();
        auto outputs = Matrix<std::complex<double>>::solve_matrix(circuit_matrix_ac, output_matrix_ac);
        outputs_mat->emplace_at(outputs->transpose()[0], i);
    }
    report(circuit, outputs_mat);
    return;
}