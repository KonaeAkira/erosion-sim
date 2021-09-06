#ifndef HEIGHTMAP_HPP
#define HEIGHTMAP_HPP

#include <ctime>
#include <iostream>

#include <QWidget>
#include <QImage>
#include <QKeyEvent>
#include <QTimer>
#include <QPainter>

#include "perlin_noise.hpp"
#include "tile.hpp"
#include "colors.hpp"

const int MAP_SIZE = 256;

class Simulation : public QWidget
{
    Q_OBJECT

public:
    Simulation(QWidget *parent);
    void simulateStep();
    QImage getTerrainImage() const;
    QImage getOverallImage() const;
    int getIteration() const;

protected:
    void paintEvent(QPaintEvent *event);

signals:
    void simulatedStep();

private:
    const siv::PerlinNoise perlin;
    Tile terrain[MAP_SIZE][MAP_SIZE];
    QImage topographic_map, precipitation_map;
    QTimer *timer;
    int iteration;
    void generateTerrain();
    double getPrecipitation(const int x, const int y);
    void updateImages();
};

#endif