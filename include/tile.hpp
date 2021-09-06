#ifndef TILE_HPP
#define TILE_HPP

#include <algorithm>
#include <numeric>

class Tile
{
    public:
        Tile();
        Tile(const double height, Tile *n, Tile *e, Tile *s, Tile *w);
        double getTotalHeight() const;
        double getTerrainHeight() const;
        double getWaterHeight() const;
        void calculateFlow();
        void doFlow();
        void doPrecipitation(const double precipitation);

    private:
        double height, water, buffer;
        Tile *neighbors[4];
};

#endif