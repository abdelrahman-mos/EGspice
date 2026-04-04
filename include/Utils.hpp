#ifndef UTILS_HPP
#define UTILS_HPP

#include <cmath>

class Utils
{
public:
    static bool ndiff2(double abstol, double reltol, double val1, double val2) {
        return (std::abs(val1 - val2) < abstol + reltol*std::abs(val1));
    }

    static bool diff_err(double abstol, double reltol, double voltage, double err) {
        return (std::abs(err) < abstol + reltol * std::abs(voltage));
    }
};


#endif /* UTILS_HPP */