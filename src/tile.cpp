#include <cassert>
#include <algorithm>
#include <numeric>
#include <iostream>

#include "tile.hpp"

Tile::Tile() {}

Tile::Tile(double height, Tile *n, Tile *e, Tile *s, Tile *w) :
    water(0.01),
    height(height),
    buffer()
{
    neighbors[0] = n;
    neighbors[1] = e;
    neighbors[2] = s;
    neighbors[3] = w;
}

double Tile::getTotalHeight() const
{
    return height + water;
}

double Tile::getTerrainHeight() const
{
    return height;
}

double Tile::getWaterHeight() const
{
    return water;
}

void Tile::calculateFlow()
{
    double flow[4] = {0, 0, 0, 0};
    for (int i = 0; i < 4; ++i)
        if (neighbors[i] != nullptr && neighbors[i]->getTotalHeight() < getTotalHeight())
            flow[i] = (getTotalHeight() - neighbors[i]->getTotalHeight()) / 4;
    const double sum = std::accumulate(flow, flow + 4, 0.0);
    if (sum > water)
        for (int i = 0; i < 4; ++i)
            flow[i] = flow[i] / sum * water;
    for (int i = 0; i < 4; ++i)
        if (neighbors[i] != nullptr)
        {
            buffer -= flow[i];
            neighbors[i]->buffer += flow[i];
        }
}

void Tile::doFlow()
{
    water += buffer;
    assert(water > -1e-9);
    water = std::max(0.0, water);
    buffer = 0;
}