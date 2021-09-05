#include <QApplication>

#include "display.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);  

    Display display;

    return app.exec();
}