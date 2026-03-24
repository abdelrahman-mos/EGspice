#ifndef CIRCUIT_HPP
#define CIRCUIT_HPP
#include <vector>
#include <memory>
#include <unordered_map>
#include "component.hpp"
#include "Command.hpp"
#include "Matrix.hpp"

class Circuit
{
    std::vector<std::unique_ptr<Component>> components;
    std::vector<std::unique_ptr<Command>> commands;
    std::shared_ptr<Matrix<double>> circuit_matrix;
    std::shared_ptr<Matrix<double>> output_matrix;
    int num_nodes;
    std::unordered_map<std::string, int> node_map;
    int num_vsources;
public:
    Circuit() {
        node_map = {{"0", 0}, {"gnd", 0}};
    }

    void add_component(std::unique_ptr<Component> component) {
        // num_nodes++;
        // if (typeid(*component) == typeid(Vsource)) num_vsources++;
        // components.push_back(std::move(component));
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
        components.push_back(std::move(component));
        num_nodes = curr_node-1;
    }

    void add_command(std::unique_ptr<Command> command) {
        commands.push_back(std::move(command));
    }

    std::shared_ptr<Matrix<double>> get_matrix() {
        return circuit_matrix;
    }

    std::shared_ptr<Matrix<double>> get_output_matrix() {
        return output_matrix;
    }

    void stamp_circuit() {
        if (circuit_matrix == nullptr) {
            circuit_matrix = std::make_shared<Matrix<double>>(num_nodes+num_vsources, num_nodes+num_vsources);
            output_matrix = std::make_shared<Matrix<double>>(num_nodes+num_vsources, 1);
        }
        for (const auto& component : components) {
            if (typeid(*component) == typeid(Vsource)) {
                Vsource* curr_vsource = dynamic_cast<Vsource*>(component.get());
                std::cout << "id: " << curr_vsource->vsource_id << std::endl;
            }
            component->stamp(circuit_matrix, output_matrix, num_vsources);
        }
    }

    void stamp_circuit(double frequency) {
        if (circuit_matrix == nullptr) {
            circuit_matrix = std::make_shared<Matrix<double>>(num_nodes, num_nodes);
            output_matrix = std::make_shared<Matrix<double>>(num_nodes, 1);
            for (const auto& component : components) {
                component->stamp(circuit_matrix, output_matrix, num_vsources, frequency);
            }
        } else {
            for (const auto& component : components) {
                // in repitition, stamp only devices that will update values in the matrix instead of stamping all devices
                if ((typeid(*component) == typeid(Inductor)) || (typeid(*component) == typeid(Capacitor))) {
                    component->stamp(circuit_matrix, output_matrix, num_vsources, frequency);
                }
            }
        }
    }
};


#endif