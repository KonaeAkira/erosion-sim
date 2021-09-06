#ifndef COLORS_HPP
#define COLORS_HPP

#include <algorithm>
#include <numeric>

#include <QColor>

class Colors
{
public:
    static QColor topographic(const double height, const double water);
    static QColor precipitation(const double precipitation);
};

#endif