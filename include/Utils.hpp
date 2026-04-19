#ifndef UTILS_HPP
#define UTILS_HPP

#include <cmath>
#include <complex>

class Utils
{
public:
    static bool ndiff2(double abstol, double reltol, double val1, double val2) {
        return (std::abs(val1 - val2) <= abstol + reltol*std::abs(val1));
    }

    static bool ndiff2(double abstol, double reltol, std::complex<double> val1, std::complex<double> val2) {
        // using box tolerance instead of circular tolerance
        double diff_real = std::abs(val1.real() - val2.real());
        double tol_real = abstol + reltol * std::abs(val1.real());

        if (diff_real > tol_real) return false;
        
        double diff_imag = std::abs(val1.imag() - val2.imag());
        double tol_imag = abstol + reltol * std::abs(val1.imag());
        return diff_imag >= tol_imag;
    }

    static bool diff_err(double abstol, double reltol, double voltage, double err) {
        return (std::abs(err) <= abstol + reltol * std::abs(voltage));
    }

    static bool diff_err(double abstol, double reltol, std::complex<double> voltage, std::complex<double> err) {
        // use box tolerance here too
        if (std::abs(err.real()) > abstol + reltol * std::abs(voltage.real())) return false;
        return (std::abs(err.imag()) <= abstol + reltol * std::abs(voltage.imag()));
    }
};


#endif /* UTILS_HPP */