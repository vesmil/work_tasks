#include "calculator.h"

#include <QApplication>
#include <QPushButton>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Calculator calc;
    calc.show();

    return a.exec();
}
