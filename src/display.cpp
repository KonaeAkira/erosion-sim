#include "display.hpp"

Display::Display(QWidget *parent) :
    QWidget(parent),
    heightmap()
{
    resize(2 * MAP_SIZE + 12, MAP_SIZE + 8);
    setWindowTitle("Hydraulic Erosion Simulator");
    show();

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&Display::simulateStep));
    timer->start(100);
}

void Display::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawImage(4, 4, heightmap.getTerrainImage());
    painter.drawImage(MAP_SIZE + 8, 4, heightmap.getOverallImage());
}

void Display::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Right)
    {
        simulateStep();
    }
}

void Display::simulateStep()
{
    heightmap.simulateStep();
    update();
}