#ifndef TILE_HPP
#define TILE_HPP

#include <vector>

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

    private:
        double water, height, buffer;
        Tile *neighbors[4];
};

#endif