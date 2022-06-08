#include <QGraphicsPixmapItem>
#include <QTextEdit>
#include <QDir>

#include "mainwindow.h"
#include "constants.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), mUi(new Ui::MainWindow)
{
    mUi->setupUi(this);

    SetupTextEdits();

    mScene = new QGraphicsScene(this);
    mUi->graphicsView->setScene(mScene);

    mGenerator = new GeneratorLink(mScene, mUi->widthEdit, mUi->heightEdit, mUi->depthEdit);

    connect(mUi->generateButton, &QPushButton::clicked, mGenerator, &GeneratorLink::handleGenerate);
    connect(mUi->saveButton, &QPushButton::clicked, mGenerator, &GeneratorLink::saveImage);

    mUi->comboBox->addItem("Default");

    QStringList palettes = glb::constants::DEFAULT_PALETTE_DIR.entryList(QStringList(), QDir::Files);
    foreach(QString filename, palettes) {
        mUi->comboBox->addItem(filename);
    }

    connect(mUi->comboBox, &QComboBox::currentTextChanged, mGenerator, &GeneratorLink::handlePalette);
}

void MainWindow::SetupTextEdits()
{
    mUi->widthEdit->setText(QString::number(glb::constants::DEFAULT_WIDTH));
    mUi->heightEdit->setText(QString::number(glb::constants::DEFAULT_HEIGHT));
    mUi->depthEdit->setText(QString::number(glb::constants::DEFAULT_ALGO_DEPTH));
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    mGenerator->ResizeFromEvent(mUi->graphicsView->viewport()->width(), mUi->graphicsView->viewport()->height());
}

MainWindow::~MainWindow()
{
    delete mUi;
}
