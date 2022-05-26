#include "mainwindow.h"
#include "qthread.h"
#include "ui_mainwindow.h"

#include <QGraphicsPixmapItem>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    generator = new FractalGenerator(width, height);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    image = new QImage{width, height, QImage::Format_RGB32};
    scene->addPixmap(QPixmap::fromImage(*image));

    generate_button = new QPushButton("Generate", this);
    generate_button->setGeometry(QRect(QPoint(350, 550), QSize(80, 30)));
    connect(generate_button, &QPushButton::released, this, &MainWindow::handleGenerate);

    generate_button = new QPushButton("Next step", this);
    generate_button->setGeometry(QRect(QPoint(440, 550), QSize(80, 30)));
    connect(generate_button, &QPushButton::released, this, &MainWindow::handleAnimate);

    generate_button = new QPushButton("Select palette", this);
    generate_button->setGeometry(QRect(QPoint(570, 550), QSize(80, 30)));
    connect(generate_button, &QPushButton::released, this, &MainWindow::handlePalette);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleGenerate()
{
    generator->Generate(*image, 50);
    scene->addPixmap(QPixmap::fromImage(*image));
}

void MainWindow::handleAnimate()
{
    generator->OneStep(*image, step++);
    scene->addPixmap(QPixmap::fromImage(*image));
}

void MainWindow::handlePalette(){
    QString filename= QFileDialog::getOpenFileName(this, "Choose Palette");
    generator->LoadPalette(filename);
}
