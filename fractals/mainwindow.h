#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "fractalgenerator.h"

#include <QGraphicsScene>
#include <QMainWindow>
#include <qpushbutton.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QGraphicsScene *scene;
    QImage *image;

    QPushButton *generate_button;
    FractalGenerator *generator;

    int width = 750;
    int height = 500;

    int step = 0;

private slots:
    void handleGenerate();
    void handleAnimate();
    void handlePalette();

};

#endif // MAINWINDOW_H
