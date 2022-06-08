#include <QFile>
#include <QThread>

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

    resultMatrix.SetThreads(mNumThreads);
    for (int i = 0; i < mNumThreads; ++i) {
        mThreads[i] = QThread::create([&, i]() {
            for (size_t iteration = 0; iteration < mDepthLimit; ++iteration)
            {
                resultMatrix.Next(i);
            }
        });

        mThreads[i]->start();
    }

    for (int i = 0; i < mNumThreads; ++i) {
        mThreads[i]->wait();
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
    mIterations.clear();

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

void ResultMatrix::Next(size_t i)
{
    // TODO remove false sharing (false invalidating of cache)
    size_t startRow = mRowsPerThread * i;

    // Double ternary expression adjusting in case of last thread and use of symmetricity...
    size_t endRow = i != mNumThreads - 1? mRowsPerThread * (i + 1) : glb::constants::USE_SYMMETRICITY? mHeight / 2 + 1: mHeight;

    for (size_t k = startRow; k < endRow; ++k)
    for (size_t j = 0; j < mWidth; ++j)
    {
        if (mInnerMatrixArray[j + k * mWidth].AboveTwo) continue;

        Result& result = mInnerMatrixArray[j + k * mWidth];
        result.Current = result.Current * result.Current + result.Original;

        if (std::abs(result.Current) > 2)
        {
            result.AboveTwo = true;
            result.LastIteration = mIterations[i];
        }
    }

    mIterations[i]++;
}

void ResultMatrix::SetThreads(int count)
{
    mNumThreads = count;

    mRowsPerThread = (mHeight / mNumThreads);
    if (glb::constants::USE_SYMMETRICITY) mRowsPerThread /= 2;

    for (int i = 0; i < count; i++) mIterations.emplace_back();
}

QImage ResultMatrix::ToImage(QRgb* palette, size_t paletteSize)
{
    QImage image(mWidth, mHeight, QImage::Format_RGB32);
    size_t maxIndex = paletteSize - 1;

    size_t height = glb::constants::USE_SYMMETRICITY? mHeight / 2 + 1 : glb::constants::USE_SYMMETRICITY;

    size_t totalIterations = mIterations[0];

    for (size_t i = 0; i < mWidth; ++i)
    {
        for (size_t j = 0; j < height; ++j)
        {
            Result& result = mInnerMatrixArray[i + j * mWidth];

            if (result.AboveTwo){
                if (glb::constants::USE_SYMMETRICITY) image.setPixel(i, mHeight - j - 1, palette[maxIndex - result.LastIteration * maxIndex / totalIterations]);
                image.setPixel(i, j, palette[maxIndex - result.LastIteration * maxIndex / totalIterations]);
            }
            else {
                if (glb::constants::USE_SYMMETRICITY) image.setPixel(i, mHeight - j - 1, palette[0]);
                image.setPixel(i, j, palette[0]);
            }
        }
    }



    return image;
}
