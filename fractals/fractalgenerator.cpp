#include "fractalgenerator.h"

#include <QFile>

FractalGenerator::FractalGenerator(int width, int height) : width(width), height(height)
{   
    palette[0] = qRgb(255,255,255);

    for (int i = 1; i < 256; ++i){
        palette[i] = qRgb(0,0,0);
    }

    for (int i = 0; i < width; ++i){
        std::vector<std::complex<double>> temp_vector;
        std::vector<std::complex<double>> zero_vector;

        for (int j = 0; j < height; ++j){
            double real = (i - 2 * width / 3.0f) / (width / 3.0f);
            double im = (j - height / 2.0f) / (height / 2.0f);
            temp_vector.emplace_back(real, im);

            zero_vector.emplace_back(0,0);
        }

        results.emplace_back(zero_vector);
        original.emplace_back(temp_vector);
    }
}

void FractalGenerator::LoadPalette(QString fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly))
        return;

    int i = 0;
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        QList<QByteArray> split = line.split(';');

        palette[i++] = qRgb(split[0].toInt(), split[1].toInt(), split[2].toInt());
    }
}

void FractalGenerator::Generate(QImage &image, int limit)
{
    image.fill(palette[0]);

    for (int iteration = 0; iteration < limit; ++iteration){
        OneStep(image, iteration);
    }
}

void FractalGenerator::OneStep(QImage &image, int iteration)
{
    if (iteration == 0) {
        image.fill(palette[0]);
    }

    for (int i = 0; i < width; ++i){
        for (int j = 0; j < height; ++j){

            results[i][j] = results[i][j] * results[i][j] + original[i][j];

            if (std::abs(results[i][j]) > 2) {
                image.setPixel(i, j, palette[255 - iteration]);

                results[i][j] = 0;
                original[i][j] = 0;
            }
        }
    }
}
