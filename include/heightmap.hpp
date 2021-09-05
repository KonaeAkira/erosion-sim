#ifndef HEIGHTMAP_HPP
#define HEIGHTMAP_HPP

#include <ctime>
#include <iostream>

#include <QImage>
#include <QKeyEvent>
#include <QTimer>

#include "perlin_noise.hpp"
#include "tile.hpp"
#include "gradient.hpp"

const int MAP_SIZE = 256;

class Heightmap
{
    public:
        Heightmap();
        void simulateStep();
        QImage getTerrainImage() const;
        QImage getOverallImage() const;

    private:
        Tile heightmap[MAP_SIZE][MAP_SIZE];
        QImage terrain_image, overall_image;
        void generateHeightmap();
        void updateImages();
};

#endif