#ifndef FRACTALGENERATOR_H
#define FRACTALGENERATOR_H

#include <complex>

#include <QGraphicsScene>
#include <QImage>
#include <QObject>
#include <QMetaType>

#include "constants.h"

struct Result {
    Result() = default;
    Result(std::complex<double> original, std::complex<double> current) : Original(original), Current(current) {}

    std::complex<double> Original;
    std::complex<double> Current;

    bool AboveTwo = false;
    size_t LastIteration;
};


struct ResultMatrix {
    ResultMatrix() = default;
    ResultMatrix(size_t width, size_t height);

    QImage ToImage(QRgb* palette, size_t paletteSize);

    void Clear();
    void Next(size_t i);
    void SetThreads(int count);

private:
    size_t mNumThreads;
    size_t mRowsPerThread;

    std::vector<size_t> mIterations;

    size_t mWidth;
    size_t mHeight;

    std::vector<Result> mInnerMatrixArray;

    double mXMin = glb::constants::X_MIN;
    double mXMax = glb::constants::X_MAX;
    double mYMin = glb::constants::Y_MIN;
    double mYMax = glb::constants::Y_MAX;
};

Q_DECLARE_METATYPE(ResultMatrix);

class GeneratorWorker : public QObject
{
    Q_OBJECT

public:
    GeneratorWorker(size_t width, size_t height, size_t depth);
    virtual ~GeneratorWorker() = default;

    ResultMatrix Generate();

private:
    static const int mNumThreads = 8;
    QThread* mThreads[mNumThreads];

    size_t mDepthLimit;
    size_t mHeight;
    size_t mWidth;

public slots:
    void generateAsync();

signals:
    void finished(ResultMatrix);
};

#endif // FRACTALGENERATOR_H
