#include <QFile>

#include "fractalgenerator.h"
#include "constants.h"

GeneratorWorker::GeneratorWorker(size_t width, size_t height, size_t depth)
{
    mWidth = width;
    mHeight = height;
    mDepthLimit = depth;
}

void GeneratorWorker::generateAsync()
{
    ResultMatrix resultMatrix{mWidth, mHeight};

    for (size_t iteration = 0; iteration < mDepthLimit; ++iteration)
    {
        resultMatrix.Next();
    }

    emit finished(resultMatrix);
}

ResultMatrix::ResultMatrix(size_t width, size_t height) : mWidth(width), mHeight(height)
{
    mInnerMatrixArray.resize(width * height);
    Clear();
}

void ResultMatrix::Clear()
{
    mIteration = 0;

    double x_step = (mXMax - mXMin) / mWidth;
    double y_step = (mYMax - mYMin) / mHeight;

    for (size_t i = 0; i < mWidth; ++i)
    {
        for (size_t j = 0; j < mHeight; ++j)
        {
            double real = mXMin + i * x_step;
            double im = mYMin + j * y_step;

            mInnerMatrixArray[i + j * mWidth] = Result{{real, im},{0,0}};
        }
    }
}

void ResultMatrix::Next()
{
    for (size_t i = 0; i < mWidth; ++i)
    {
        for (size_t j = 0; j < mHeight; ++j)
        {
            if (!mInnerMatrixArray[i + j * mWidth].AboveTwo)
            {
                Result& result = mInnerMatrixArray[i + j * mWidth];
                result.Current = result.Current * result.Current + result.Original;

                if (std::abs(result.Current) > 2)
                {
                    result.AboveTwo = true;
                    result.LastIteration = mIteration;
                }
            }
        }
    }

    mIteration++;
}

QImage ResultMatrix::ToImage(QRgb* palette, size_t paletteSize)
{
    QImage image(mWidth, mHeight, QImage::Format_RGB32);
    size_t maxIndex = paletteSize - 1;

    for (size_t i = 0; i < mWidth; ++i)
    {
        for (size_t j = 0; j < mHeight; ++j)
        {
            Result& result = mInnerMatrixArray[i + j * mWidth];

            if (result.AboveTwo) image.setPixel(i, j, palette[maxIndex - result.LastIteration * maxIndex / mIteration]);
            else image.setPixel(i, j, palette[0]);
        }
    }

    return image;
}
