#ifndef GRADIENT_HPP
#define GRADIENT_HPP

#include <cmath>
#include <algorithm>

#include <QColor>

class Gradient
{
    public:
        static QColor getColor(double height);
    
    private:
        static const char gradient[127][8];
};

#endif