#include "colors.hpp"

QColor Colors::topographic(const double height, const double water)
{
    const std::pair<double, QColor> ramp[] = 
    {
        std::make_pair(0.0000, QColor::fromRgb(113,171,216)),
        std::make_pair(0.0345, QColor::fromRgb(121,178,222)),
        std::make_pair(0.0690, QColor::fromRgb(132,185,227)),
        std::make_pair(0.1034, QColor::fromRgb(141,193,234)),
        std::make_pair(0.1379, QColor::fromRgb(150,201,240)),
        std::make_pair(0.1724, QColor::fromRgb(161,210,247)),
        std::make_pair(0.2069, QColor::fromRgb(172,219,251)),
        std::make_pair(0.2414, QColor::fromRgb(185,227,255)),
        std::make_pair(0.2759, QColor::fromRgb(198,236,255)),
        std::make_pair(0.3103, QColor::fromRgb(216,242,254)), // water
        std::make_pair(0.3448, QColor::fromRgb(172,208,165)), // land
        std::make_pair(0.3793, QColor::fromRgb(148,191,139)),
        std::make_pair(0.4138, QColor::fromRgb(168,198,143)),
        std::make_pair(0.4483, QColor::fromRgb(189,204,150)),
        std::make_pair(0.4828, QColor::fromRgb(209,215,171)),
        std::make_pair(0.5172, QColor::fromRgb(225,228,181)),
        std::make_pair(0.5517, QColor::fromRgb(239,235,192)),
        std::make_pair(0.5862, QColor::fromRgb(232,225,182)),
        std::make_pair(0.6207, QColor::fromRgb(222,214,163)),
        std::make_pair(0.6552, QColor::fromRgb(211,202,157)),
        std::make_pair(0.6897, QColor::fromRgb(202,185,130)),
        std::make_pair(0.7241, QColor::fromRgb(195,167,107)),
        std::make_pair(0.7586, QColor::fromRgb(185,152, 90)),
        std::make_pair(0.7931, QColor::fromRgb(170,135, 83)),
        std::make_pair(0.8276, QColor::fromRgb(172,154,124)),
        std::make_pair(0.8621, QColor::fromRgb(186,174,154)),
        std::make_pair(0.8966, QColor::fromRgb(202,195,184)),
        std::make_pair(0.9310, QColor::fromRgb(224,222,216)),
        std::make_pair(0.9655, QColor::fromRgb(245,244,242))
    };

    if (height >= 0.3448 && water >= 1e-3)
        return QColor::fromRgb(216,242,254);
    else if (height < 0.3448 && water < 1e-3)
        return QColor::fromRgb(172,208,165);

    int i = sizeof(ramp) / sizeof(ramp[0]) - 1;
    while (i > 0 && ramp[i].first > height) --i;
    return ramp[i].second;
}

QColor Colors::precipitation(const double precipitation)
{
    const int v = 255 * std::clamp(precipitation, 0.0, 1.0);
    return QColor::fromRgb(v, v, v);
}