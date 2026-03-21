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
    double ac_val;
public:
    Component(std::vector<int> terminals, std::string name = "", double value = 0.0) : terminals(terminals), name(name), value(value), ac_val(value) {}
    Component(std::vector<int> terminals, double ac_val, std::string name = "", double value = 0.0) : terminals(terminals), name(name), value(value), ac_val(ac_val) {}

    virtual void stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, int num_vsources) {
        return;
    }

    virtual void stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, int num_vsources, double frequency) {
        return;
    }
};

class Vsource : public Component {
public:
    int vsource_id;
    Vsource(std::vector<int> terminals, int vsource_id, std::string name = "", double value = 0.0) : Component(terminals, name, value), vsource_id(vsource_id) {}
    Vsource(std::vector<int> terminals, int vsource_id, double ac_val, std::string name = "", double value = 0.0) : Component(terminals, ac_val, name, value), vsource_id(vsource_id) {}
    void stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, int num_vsources) override;
    void stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, int num_vsources, double frequency) override;
};

class Isource : public Component
{
public:
    Isource(std::vector<int> terminals, std::string name = "", double value = 0.0) : Component(terminals, name, value) {}
    Isource(std::vector<int> terminals, double ac_val, std::string name = "", double value = 0.0) : Component(terminals, ac_val, name, value) {}
    void stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, int num_vsources) override;
    void stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, int num_vsources, double frequency) override;
};


class Resistor : public Component {
public:
    Resistor(std::vector<int> terminals, std::string name = "", double value = 0.0) : Component(terminals, name, value) {}
    Resistor(std::vector<int> terminals, double ac_val, std::string name = "", double value = 0.0) : Component(terminals, ac_val, name, value) {}
    void stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, int num_vsources) override;
    void stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, int num_vsources, double frequency) override;
};

class Inductor : public Component {
public:
    Inductor(std::vector<int> terminals, std::string name = "", double value = 0.0) : Component(terminals, name, value) {}
    void stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, int num_vsources) override;
    void stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, int num_vsources, double frequency) override;
};

class Capacitor : public Component {
public:
    Capacitor(std::vector<int> terminals, std::string name = "", double value = 0.0) : Component(terminals, name, value) {}
    void stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, int num_vsources) override;
    void stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, int num_vsources, double frequency) override;
};

#endif