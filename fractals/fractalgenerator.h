#ifndef FRACTALGENERATOR_H
#define FRACTALGENERATOR_H

#include <QGraphicsScene>
#include <QImage>

#include <complex>

class FractalGenerator
{
public:
    FractalGenerator(int width, int height);

    void Generate(QImage &image, int limit);
    void OneStep(QImage &image, int iteration);

    void LoadPalette(QString fileName);

private:
    QRgb palette[256];

    int width;
    int height;

    std::vector<std::vector<std::complex<double>>> original;
    std::vector<std::vector<std::complex<double>>> results;
};

#endif // FRACTALGENERATOR_H
