#include "heightmap.hpp"

Heightmap::Heightmap() :
    terrain_image(MAP_SIZE, MAP_SIZE, QImage::Format_RGB32),
    overall_image(MAP_SIZE, MAP_SIZE, QImage::Format_RGB32)
{
    generateHeightmap();
}

void Heightmap::generateHeightmap()
{
    const double frequency = 4.0;
    const std::int32_t octaves = 8;
    const std::uint32_t seed = time(nullptr);

    const siv::PerlinNoise perlin(seed);
    const double fx = MAP_SIZE / frequency;
    const double fy = MAP_SIZE / frequency;

    for (int i = 0; i < MAP_SIZE; ++i)
        for (int j = 0; j < MAP_SIZE; ++j)
        {
            Tile *n = j == 0 ? nullptr : &heightmap[i][j - 1];
            Tile *e = i + 1 == MAP_SIZE ? nullptr : &heightmap[i + 1][j];
            Tile *s = j + 1 == MAP_SIZE ? nullptr : &heightmap[i][j + 1];
            Tile *w = i == 0 ? nullptr : &heightmap[i - 1][j];
            heightmap[i][j] = Tile(perlin.accumulatedOctaveNoise2D_0_1(i / fx, j / fy, octaves), n, e, s, w);
        }

    updateImages();
}

void Heightmap::updateImages()
{
    for (int i = 0; i < MAP_SIZE; ++i)
        for (int j = 0; j < MAP_SIZE; ++j)
        {
            const double h = heightmap[i][j].getTerrainHeight();
            terrain_image.setPixelColor(i, j, Gradient::getColor(h));
        }

    for (int i = 0; i < MAP_SIZE; ++i)
        for (int j = 0; j < MAP_SIZE; ++j)
            if (heightmap[i][j].getWaterHeight() > 1e-3)
                overall_image.setPixelColor(i, j, "#00ffff");
            else
            {
                const double h = heightmap[i][j].getTerrainHeight();
                overall_image.setPixelColor(i, j, Gradient::getColor(h));
            }
}

void Heightmap::simulateStep()
{
    for (int i = 0; i < MAP_SIZE; ++i)
        for (int j = 0; j < MAP_SIZE; ++j)
            heightmap[i][j].calculateFlow();

    for (int i = 0; i < MAP_SIZE; ++i)
        for (int j = 0; j < MAP_SIZE; ++j)
            heightmap[i][j].doFlow();

    updateImages();
}

QImage Heightmap::getTerrainImage() const { return terrain_image; }
QImage Heightmap::getOverallImage() const { return overall_image; }