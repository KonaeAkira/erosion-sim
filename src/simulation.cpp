#include "simulation.hpp"

Simulation::Simulation(QWidget *parent) :
    QWidget(parent),
    perlin(123),
    topographic_map(MAP_SIZE, MAP_SIZE, QImage::Format_RGB32),
    precipitation_map(MAP_SIZE, MAP_SIZE, QImage::Format_RGB32),
    iteration(0)
{
    setFixedSize(2 * MAP_SIZE + 12, MAP_SIZE + 8);
    generateTerrain();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&Simulation::simulateStep));
    timer->start(0);
}

void Simulation::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawImage(4, 4, getTerrainImage());
    painter.drawImage(MAP_SIZE + 8, 4, getOverallImage());
}

void Simulation::generateTerrain()
{
    const double frequency = 1.0;
    const std::int32_t octaves = 8;

    const double fx = MAP_SIZE / frequency;
    const double fy = MAP_SIZE / frequency;

    for (int i = 0; i < MAP_SIZE; ++i)
        for (int j = 0; j < MAP_SIZE; ++j)
        {
            Tile *n = j == 0 ? nullptr : &terrain[i][j - 1];
            Tile *e = i + 1 == MAP_SIZE ? nullptr : &terrain[i + 1][j];
            Tile *s = j + 1 == MAP_SIZE ? nullptr : &terrain[i][j + 1];
            Tile *w = i == 0 ? nullptr : &terrain[i - 1][j];
            terrain[i][j] = Tile(perlin.accumulatedOctaveNoise2D_0_1(i / fx, j / fy, octaves), n, e, s, w);
        }

    updateImages();
}

double Simulation::getPrecipitation(const int x, const int y)
{
    const double frequency = 1.0;
    const std::int32_t octaves = 8;

    const double fx = MAP_SIZE / frequency;
    const double fy = MAP_SIZE / frequency;
    const double fz = MAP_SIZE / frequency;

    return perlin.accumulatedOctaveNoise3D_0_1(x / fx, y / fy, getIteration() / fz, octaves);
}

void Simulation::updateImages()
{
    for (int i = 0; i < MAP_SIZE; ++i)
        for (int j = 0; j < MAP_SIZE; ++j)
        {
            const Tile &tile = terrain[i][j];
            topographic_map.setPixelColor(i, j, Colors::topographic(tile.getTerrainHeight(), tile.getWaterHeight()));
        }

    for (int i = 0; i < MAP_SIZE; ++i)
        for (int j = 0; j < MAP_SIZE; ++j)
            precipitation_map.setPixelColor(i, j, Colors::precipitation(getPrecipitation(i, j)));
}

void Simulation::simulateStep()
{
    for (int i = 0; i < MAP_SIZE; ++i)
        for (int j = 0; j < MAP_SIZE; ++j)
            terrain[i][j].calculateFlow();

    for (int i = 0; i < MAP_SIZE; ++i)
        for (int j = 0; j < MAP_SIZE; ++j)
            terrain[i][j].calculateErosion();

    for (int i = 0; i < MAP_SIZE; ++i)
        for (int j = 0; j < MAP_SIZE; ++j)
            terrain[i][j].doErosion();

    for (int i = 0; i < MAP_SIZE; ++i)
        for (int j = 0; j < MAP_SIZE; ++j)
            terrain[i][j].addWater(getPrecipitation(i, j) * 1e-5);

    updateImages();
    ++iteration;
    emit simulatedStep();
}

QImage Simulation::getTerrainImage() const { return topographic_map; }
QImage Simulation::getOverallImage() const { return precipitation_map; }
int Simulation::getIteration() const { return iteration; }