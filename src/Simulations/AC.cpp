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

void AC::run(std::shared_ptr<Circuit> circuit) {
    for (auto freq : frequency_points) {
        std::cout << "frequency: " << freq << std::endl;
        circuit->stamp_circuit(freq);
        auto circuit_matrix = circuit->get_matrix();
        auto output_matrix = circuit->get_output_matrix();
        auto outputs = Matrix<double>::solve_matrix(circuit_matrix, output_matrix);
        // std::cout << outputs << std::endl;
    }
    return;
}