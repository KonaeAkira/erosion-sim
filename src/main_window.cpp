#include "main_window.hpp"

MainWindow::MainWindow() :
    heightmap(new Simulation(this))
{
    setCentralWidget(heightmap);
    connect(heightmap, &Simulation::simulatedStep, this, &MainWindow::simulatedStepListener);
    
    setWindowTitle("Hydraulic Erosion Simulation");
    show();
}

void MainWindow::simulatedStepListener()
{
    statusBar()->showMessage("Iteration " + QString::number(heightmap->getIteration()));
    update();
}