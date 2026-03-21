#ifndef COMPONENT_HPP
#define COMPONENT_HPP
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include "Matrix.hpp"

class Component
{
protected:
    std::vector<int> terminals;
    std::string name;
    double value;
public:
    Component(std::vector<int> terminals, std::string name = "", double value = 0.0) : terminals(terminals), name(name), value(value) {}

    virtual void stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix) {
        return;
    }

    virtual void stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, double frequency) {
        return;
    }
};

class Vsource : public Component {
public:
    Vsource(std::vector<int> terminals, std::string name = "", double value = 0.0) : Component(terminals, name, value) {}
    void stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix) override;
    void stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, double frequency) override;
};

class Isource : public Component
{
public:
    Isource(std::vector<int> terminals, std::string name = "", double value = 0.0) : Component(terminals, name, value) {}
    void stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix) override;
    void stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, double frequency) override;
};


class Resistor : public Component {
public:
    Resistor(std::vector<int> terminals, std::string name = "", double value = 0.0) : Component(terminals, name, value) {}
    void stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix) override;
    void stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, double frequency) override;
};

class Inductor : public Component {
public:
    Inductor(std::vector<int> terminals, std::string name = "", double value = 0.0) : Component(terminals, name, value) {}
    void stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix) override;
    void stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, double frequency) override;
};

class Capacitor : public Component {
public:
    Capacitor(std::vector<int> terminals, std::string name = "", double value = 0.0) : Component(terminals, name, value) {}
    void stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix) override;
    void stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, double frequency) override;
};

#endif