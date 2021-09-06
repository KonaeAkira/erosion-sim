#include "tile.hpp"

Tile::Tile() {}

Tile::Tile(double height, Tile *n, Tile *e, Tile *s, Tile *w) :
    height(height),
    // water(0.0),
    water(std::max(0.0, 0.3200 - height)),
    sediment(0.0),
    height_buffer(0.0)
{
    neighbors[0] = n;
    neighbors[1] = e;
    neighbors[2] = s;
    neighbors[3] = w;
}

double Tile::getTotalHeight() const { return height + water; }
double Tile::getTerrainHeight() const { return height; }
double Tile::getWaterHeight() const { return water; }
double Tile::getSediment() const { return sediment; }

void Tile::addWater(const double water) { this->water += water; }
void Tile::addSediment(const double sediment) { this->sediment += sediment; }

void Tile::calculateFlow()
{
    std::fill(water_out, water_out + 4, 0.0);
    for (int i = 0; i < 4; ++i)
        if (neighbors[i] != nullptr && neighbors[i]->getTotalHeight() < getTotalHeight())
            water_out[i] = (getTotalHeight() - neighbors[i]->getTotalHeight()) / 4;

    const double total_water_out = std::accumulate(water_out, water_out + 4, 0.0);
    if (total_water_out > 0.0 && (total_water_out > water || water < 1e-4))
    {
        const double t = water / total_water_out;
        for (int i = 0; i < 4; ++i)
            water_out[i] *= t;
    }

    std::fill(sediment_out, sediment_out + 4, 0.0);
    for (int i = 0; i < 4; ++i)
        if (neighbors[i] != nullptr && neighbors[i]->getTerrainHeight() < getTotalHeight() && neighbors[i]->getSediment() < getSediment())
            sediment_out[i] = (getSediment() - neighbors[i]->getSediment()) / 4;
    
    const double total_sediment_out = std::accumulate(sediment_out, sediment_out + 4, 0.0);
    if (total_sediment_out > sediment)
    {
        const double t = sediment / total_sediment_out;
        for (int i = 0; i < 4; ++i)
            sediment_out[i] *= t;
    }
}

void Tile::calculateErosion()
{
    for (int i = 0; i < 4; ++i)
    {
        water -= water_out[i];
        sediment -= sediment_out[i];
        if (neighbors[i] != nullptr)
        {
            water += neighbors[i]->water_out[(i + 2) & 0b11];
            sediment += neighbors[i]->sediment_out[(i + 2) & 0b11];
        }
    }

    // force-based erosion
    double fx = 0.0, fy = 0.0;
    if (neighbors[0] != nullptr) fy += neighbors[0]->water_out[2];
    if (neighbors[1] != nullptr) fx -= neighbors[1]->water_out[3];
    if (neighbors[2] != nullptr) fy -= neighbors[2]->water_out[0];
    if (neighbors[3] != nullptr) fx += neighbors[3]->water_out[1];

    const double force_c = 10;

    if (fx > 0 && neighbors[1] != nullptr && getTotalHeight() < neighbors[1]->getTerrainHeight())
    {
        const double t = std::min((neighbors[1]->getTerrainHeight() - getTotalHeight()) / 4, fx * force_c);
        sediment += t;
        neighbors[1]->height_buffer -= t;
    }
    if (fy > 0 && neighbors[2] != nullptr && getTotalHeight() < neighbors[2]->getTerrainHeight())
    {
        const double t = std::min((neighbors[2]->getTerrainHeight() - getTotalHeight()) / 4, fy * force_c);
        sediment += t;
        neighbors[2]->height_buffer -= t;
    }
    if (fx < 0 && neighbors[3] != nullptr && getTotalHeight() < neighbors[3]->getTerrainHeight())
    {
        const double t = std::min((neighbors[3]->getTerrainHeight() - getTotalHeight()) / 4, (-fx) * force_c);
        sediment += t;
        neighbors[3]->height_buffer -= t;
    }
    if (fy < 0 && neighbors[0] != nullptr && getTotalHeight() < neighbors[0]->getTerrainHeight())
    {
        const double t = std::min((neighbors[0]->getTerrainHeight() - getTotalHeight()) / 4, (-fy) * force_c);
        sediment += t;
        neighbors[0]->height_buffer -= t;
    }

    // dissolution-based erosion
    if (water > 0)
    {
        double f = 0.0;
        for (int i = 0; i < 4; ++i)
            if (neighbors[i] != nullptr)
                f += neighbors[i]->water_out[(i + 2) & 0b11];
        f /= std::max(1e-3, water);
        const double t = f * 1e-5;
        sediment += t;
        height_buffer -= t;
    }

    // account for rounding errors
    water = std::max(0.0, water - 3e-6);
    sediment = std::max(0.0, sediment);
}

void Tile::doErosion()
{
    const double t = sediment * 0.1;
    sediment -= t;
    height += height_buffer + t;
    height_buffer = 0;
}