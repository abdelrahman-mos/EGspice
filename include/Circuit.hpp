#ifndef CIRCUIT_HPP
#define CIRCUIT_HPP
#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>
#include <complex>
#include "component.hpp"
#include "Command.hpp"
#include "Matrix.hpp"

class Circuit
{
    std::vector<std::unique_ptr<Component>> components_;
    std::vector<std::unique_ptr<Command>> commands_;
    std::shared_ptr<Matrix<double>> circuit_matrix;
    std::shared_ptr<Matrix<double>> output_matrix;
    std::shared_ptr<Matrix<std::complex<double>>> circuit_matrix_ac;
    std::shared_ptr<Matrix<std::complex<double>>> output_matrix_ac;
    int num_nodes;
    std::unordered_map<std::string, int> node_map;
    int num_vsources;
    int num_inductors;
    bool freq_first_point;
public:
    Circuit() {
        node_map = {{"0", 0}, {"gnd", 0}};
        freq_first_point = true;
        num_nodes = 0;
        num_inductors = 0;
        num_vsources = 0;
    }

    int numNodes() const {
        return num_nodes;
    }

    int numVsources() const {
        return num_vsources;
    }

    int numInductors() const {
        return num_inductors;
    }

    std::vector<std::unique_ptr<Component>>& components() {
        return components_;
    }

    std::vector<std::unique_ptr<Command>>& commands() {
        return commands_;
    }

    void add_component(std::unique_ptr<Component> component) {
        static int curr_node = 1;
        std::vector<std::string> terminals = component->get_terminals();
        std::vector<int> terminals_int;
        for (auto terminal : terminals) {
            auto curr_terminal = node_map.find(terminal);
            if (curr_terminal == node_map.end()) {
                terminals_int.push_back(curr_node);
                node_map.insert({terminal, curr_node++});
            } else {
                terminals_int.push_back(curr_terminal->second);
            }
        }
        component->update_terminals(terminals_int);
        if (typeid(*component) == typeid(Vsource)) num_vsources++;
        if (typeid(*component) == typeid(Inductor)) num_inductors++;
        components_.push_back(std::move(component));
        num_nodes = curr_node-1;
    }

    void add_command(std::unique_ptr<Command> command) {
        commands_.push_back(std::move(command));
    }

    std::shared_ptr<Matrix<double>> get_matrix() {
        return circuit_matrix;
    }

    std::shared_ptr<Matrix<double>> get_output_matrix() {
        return output_matrix;
    }

    std::shared_ptr<Matrix<std::complex<double>>> get_ac_matrix() {
        return circuit_matrix_ac;
    }

    std::shared_ptr<Matrix<std::complex<double>>> get_ac_output_matrix() {
        return output_matrix_ac;
    }

    void stamp_circuit() {
        if (circuit_matrix == nullptr) {
            circuit_matrix = std::make_shared<Matrix<double>>(num_nodes+num_vsources+num_inductors, num_nodes+num_vsources+num_inductors);
            output_matrix = std::make_shared<Matrix<double>>(num_nodes+num_vsources+num_inductors, 1);
        }
        for (const auto& component : components_) {
            if (typeid(*component) == typeid(Vsource)) {
                Vsource* curr_vsource = dynamic_cast<Vsource*>(component.get());
                std::cout << "id: " << curr_vsource->vsource_id << std::endl;
            }
            component->stamp(circuit_matrix, output_matrix, num_vsources, num_inductors);
        }
    }

    void stamp_circuit(double frequency) {
        if ((circuit_matrix_ac == nullptr) || (freq_first_point == true)) {
            circuit_matrix_ac = std::make_shared<Matrix<std::complex<double>>>(num_nodes+num_vsources, num_nodes+num_vsources);
            output_matrix_ac = std::make_shared<Matrix<std::complex<double>>>(num_nodes+num_vsources, 1);
            for (const auto& component : components_) {
                component->stamp(circuit_matrix_ac, output_matrix_ac, num_vsources, frequency);
            }
            freq_first_point = false;
        } else {
            for (const auto& component : components_) {
                // in repitition, stamp only devices that will update values in the matrix instead of stamping all devices
                if ((typeid(*component) == typeid(Inductor)) || (typeid(*component) == typeid(Capacitor))) {
                    component->stamp(circuit_matrix_ac, output_matrix_ac, num_vsources, frequency);
                }
            }
        }
    }
};


#endif