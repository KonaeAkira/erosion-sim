#ifndef DISPLAYHPP
#define DISPLAY_HPP

#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QKeyEvent>

#include "heightmap.hpp"

class Display : public QWidget
{
    public:
        Display(QWidget *parent = nullptr);
        void simulateStep();

    protected:
        void paintEvent(QPaintEvent *event);
        void keyPressEvent(QKeyEvent *event);

    private:
        Heightmap heightmap;
};

#endif