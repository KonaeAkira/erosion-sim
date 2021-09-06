#ifndef TILE_HPP
#define TILE_HPP

#include <algorithm>
#include <numeric>
#include <iostream>

class Tile
{
    public:
        Tile();
        Tile(const double height, Tile *n, Tile *e, Tile *s, Tile *w);

        double getTotalHeight() const;
        double getTerrainHeight() const;
        double getWaterHeight() const;
        double getSediment() const;

        void addWater(const double water);
        void addSediment(const double sediment);

        void calculateFlow();
        void calculateErosion();
        void doErosion();

    private:
        double height, water, sediment;
        Tile *neighbors[4];
        double height_buffer, water_out[4], sediment_out[4];
};

#endif