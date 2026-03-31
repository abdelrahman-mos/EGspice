#ifndef COMMAND_HPP
#define COMMAND_HPP
#include <string>
#include <vector>
#include <cmath>
#include <memory>
#include "Logger.hpp"
#include "Matrix.hpp"

class Circuit;

class Command {
protected:
    std::string name_;
    std::vector<std::string> command_options;
    std::shared_ptr<Logger> logger_;
public:
    Command(std::shared_ptr<Logger> logger) : logger_(logger) {}
    Command(std::string name, std::shared_ptr<Logger> logger) : name_(name), logger_(logger) {}
    Command(std::string name, std::vector<std::string> command_options, std::shared_ptr<Logger> logger) : name_(name), command_options(command_options), logger_(logger) {}

    std::string name() const {
        return name_;
    }
    
    virtual void run(std::shared_ptr<Circuit> circuit, std::shared_ptr<Matrix<double>> coeff, std::shared_ptr<Matrix<double>> free_term) {
        return;
    }
    virtual void run(std::shared_ptr<Circuit> circuit, std::shared_ptr<Matrix<std::complex<double>>> coeff, std::shared_ptr<Matrix<std::complex<double>>> free_term) {
        return;
    }
};

class Simulation : public Command {
protected:
    bool first_point;
public:
    Simulation(std::string name, std::shared_ptr<Logger> logger) : Command(name, logger) {
        first_point = true;
    }
    virtual void stamp(std::shared_ptr<Circuit> circuit, std::shared_ptr<Matrix<double>>& coeff, std::shared_ptr<Matrix<double>>& free_term) {
        return;
    }
    virtual void stamp(std::shared_ptr<Circuit> circuit, std::shared_ptr<Matrix<std::complex<double>>>& coeff, std::shared_ptr<Matrix<std::complex<double>>>& free_term, double freq) {
        return;
    }
    virtual void report(std::shared_ptr<Circuit> circuit, std::shared_ptr<Matrix<double>> outputs) {
        return;
    }
    virtual void report(std::shared_ptr<Circuit> circuit, std::shared_ptr<Matrix<std::complex<double>>> outputs) {
        return;
    }
};

class OP : public Simulation {
public:
    OP(std::string name, std::shared_ptr<Logger> logger) : Simulation(name, logger) {}
    void stamp(std::shared_ptr<Circuit> circuit, std::shared_ptr<Matrix<double>>& coeff, std::shared_ptr<Matrix<double>>& free_term) override;
    void report(std::shared_ptr<Circuit> circuit, std::shared_ptr<Matrix<double>> outputs) override;
    void run(std::shared_ptr<Circuit> circuit, std::shared_ptr<Matrix<double>> coeff, std::shared_ptr<Matrix<double>> free_term) override;
};

enum class AC_TYPE {
    DEC,
    OCT,
    LIN
};

class AC : public Simulation {
    double fstart;
    double fend;
    size_t numpoints;
    AC_TYPE type;
    std::vector<double> frequency_points;
    void expand_freq();
    void expand_freq_dec();
    void expand_freq_oct();
    void expand_freq_lin();
public:
    AC(std::string name, double fstart, double fend, size_t numpoints, AC_TYPE type, std::shared_ptr<Logger> logger) : Simulation(name, logger), fstart(fstart),
     fend(fend), numpoints(numpoints), type(type) {
        expand_freq();
        first_point = true;
    }
    void stamp(std::shared_ptr<Circuit> circuit, std::shared_ptr<Matrix<std::complex<double>>>& coeff, std::shared_ptr<Matrix<std::complex<double>>>& free_term, double freq) override;
    void run(std::shared_ptr<Circuit> circuit, std::shared_ptr<Matrix<std::complex<double>>> coeff, std::shared_ptr<Matrix<std::complex<double>>> free_term) override;
    virtual void report(std::shared_ptr<Circuit> circuit, std::shared_ptr<Matrix<std::complex<double>>> outputs) override;
};

#endif