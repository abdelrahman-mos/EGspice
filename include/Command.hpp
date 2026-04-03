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

    // written by ai
    static std::string to_spice_engineering(double value, int precision = 5) {
        // 1. Handle exact zero gracefully (No suffix needed)
        if (value == 0.0) {
            std::ostringstream zero_stream;
            zero_stream << std::fixed << std::setprecision(precision) << 0.0;
            return zero_stream.str();
        }

        // 2. Calculate the engineering exponent (multiples of 3)
        double exp = std::floor(std::log10(std::abs(value)));
        int eng_exp = static_cast<int>(std::floor(exp / 3.0) * 3);
        
        // 3. Calculate the mantissa
        double mantissa = value / std::pow(10, eng_exp);

        // 4. Start building the string
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(precision) << mantissa;

        // 5. The SPICE Suffix Router
        switch (eng_exp) {
            case -18: oss << "a"; break;    // atto
            case -15: oss << "f"; break;    // femto
            case -12: oss << "p"; break;    // pico
            case -9:  oss << "n"; break;    // nano
            case -6:  oss << "u"; break;    // micro
            case -3:  oss << "m"; break;    // milli
            case 0:   break;                // base unit (no suffix)
            case 3:   oss << "K"; break;    // kilo
            case 6:   oss << "MEG"; break;  // mega
            case 9:   oss << "G"; break;    // giga
            case 12:  oss << "T"; break;    // tera
            default:
                // 6. The Fallback: If it's astronomically huge or tiny, use 'e'
                oss << "e" << (eng_exp >= 0 ? "+" : "-") 
                    << std::setfill('0') << std::setw(2) << std::abs(eng_exp);
                break;
        }

        return oss.str();
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
    virtual void stamp(std::shared_ptr<Circuit> circuit, std::shared_ptr<Matrix<std::complex<double>>>& coeff, 
        std::shared_ptr<Matrix<std::complex<double>>>& free_term, double freq, double prev_freq=0.0) {
        return;
    }
    virtual void stamp(std::shared_ptr<Circuit> circuit, std::shared_ptr<Matrix<double>>& coeff, std::shared_ptr<Matrix<double>>& free_term, 
        double outer_value, double prev_outer_value, double inner_value, double prev_inner_value) {
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
    void stamp(std::shared_ptr<Circuit> circuit, std::shared_ptr<Matrix<std::complex<double>>>& coeff, std::shared_ptr<Matrix<std::complex<double>>>& free_term, double freq, double prev_freq=0.0) override;
    void run(std::shared_ptr<Circuit> circuit, std::shared_ptr<Matrix<std::complex<double>>> coeff, std::shared_ptr<Matrix<std::complex<double>>> free_term) override;
    virtual void report(std::shared_ptr<Circuit> circuit, std::shared_ptr<Matrix<std::complex<double>>> outputs) override;
};

class DC : public Simulation {
    double start_outer;
    double end_outer;
    double step_outer;
    double start_inner;
    double end_inner;
    double step_inner;
    std::string inner_vsource_name;
    std::string outer_vsource_name;
    std::vector<double> outer_points;
    std::vector<double> inner_points;
    void expand_points();
public:
    DC(std::string name, double start_outer, double end_outer, double step_outer, std::string outer_vsource_name, std::shared_ptr<Logger> logger, 
        double start_inner = INFINITY, double end_inner = INFINITY, double step_inner = INFINITY, std::string inner_vsource_name = "") : 
        Simulation(name, logger), start_outer(start_outer), end_outer(end_outer), step_outer(std::abs(step_outer)), start_inner(start_inner), 
        end_inner(end_inner), step_inner(std::abs(step_inner)), inner_vsource_name(inner_vsource_name), outer_vsource_name(outer_vsource_name) {
            expand_points();
        }
    void stamp(std::shared_ptr<Circuit> circuit, std::shared_ptr<Matrix<double>>& coeff, std::shared_ptr<Matrix<double>>& free_term, 
        double outer_value, double prev_outer_value, double inner_value, double prev_inner_value) override;
    void report(std::shared_ptr<Circuit> circuit, std::shared_ptr<Matrix<double>> outputs) override;
    void report_curr_idx(std::shared_ptr<Circuit> circuit, std::shared_ptr<Matrix<double>> outputs, std::stringstream& message, size_t curr_idx, std::string pre_text = "");
    void run(std::shared_ptr<Circuit> circuit, std::shared_ptr<Matrix<double>> coeff, std::shared_ptr<Matrix<double>> free_term) override;
};

#endif