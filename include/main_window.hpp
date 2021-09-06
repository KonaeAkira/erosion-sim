#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QWidget>
#include <QMainWindow>
#include <QImage>
#include <QPainter>
#include <QKeyEvent>
#include <QStatusBar>

#include "simulation.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

public slots:
    void simulatedStepListener();

private:
    Simulation *heightmap;
};

#endif