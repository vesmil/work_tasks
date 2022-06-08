#include <QFileDialog>
#include <QThread>
#include <QGraphicsView>

#include "constants.h"
#include "generatorlink.h"

GeneratorLink::GeneratorLink(QGraphicsScene *scene, QTextEdit *widthText, QTextEdit *heightText, QTextEdit *depthText)
    : mCurrentResult(), mConnectedScreen(scene), mWidthText(widthText), mHeightText(heightText), mDepthText(depthText)
{
    // UI init
    LoadTextValues();

    mCurrentImage = QImage{mWidth, mHeight, QImage::Format_RGB32};
    scene->addPixmap(QPixmap::fromImage(mCurrentImage));

    LoadDefaultPalette();

    qRegisterMetaType<ResultMatrix>();
}

void GeneratorLink::LoadTextValues()
{
    int tempHeight = mHeight;
    int tempWidth = mWidth;
    bool ok;

    mHeight = mHeightText->toPlainText().toInt(&ok);
    if (!ok)
    {
        mHeight = tempHeight == 0 ? glb::constants::DEFAULT_HEIGHT : tempHeight;
        mHeightText->setText(QString::number(mHeight));
    }

    mWidth = mWidthText->toPlainText().toInt(&ok);
    if (!ok)
    {
        mWidth = tempWidth == 0 ? glb::constants::DEFAULT_WIDTH : tempWidth;
        mWidthText->setText(QString::number(mWidth));
    }

    mDepth = mDepthText->toPlainText().toInt(&ok);
    if (!ok)
    {
        mDepth = glb::constants::DEFAULT_ALGO_DEPTH;
        mDepthText->setText(QString::number(mDepth));
    }


    if (mHeight != tempHeight || mWidth != tempWidth)
    {
        mCurrentImage = QImage{mWidth, mHeight, QImage::Format_RGB32};
    }
}

void GeneratorLink::handleGenerate()
{
    LoadTextValues();

    GeneratorWorker* worker = new GeneratorWorker(mWidth, mHeight, mDepth);
    QThread* thread = new QThread();

    worker->moveToThread(thread);
    connect(thread, &QThread::started, worker, &GeneratorWorker::generateAsync);
    connect(worker, &GeneratorWorker::finished, this, &GeneratorLink::imageGenerated);
    connect(worker, &GeneratorWorker::finished, worker, &GeneratorWorker::deleteLater);
    connect(worker, &GeneratorWorker::destroyed, thread, &QThread::quit);

    thread->start();
}

void GeneratorLink::imageGenerated(ResultMatrix result)
{
    mCurrentResult = result;

    QImage image = result.ToImage(mPalette, sizeof(mPalette)/sizeof(*mPalette));
    mConnectedScreen->clear();
    mConnectedScreen->addPixmap(QPixmap::fromImage(image));
}

bool GeneratorLink::LoadPalette(QFile&& file)
{
    if (!file.open(QIODevice::ReadOnly)) return false;

    int i = 0;
    while (!file.atEnd())
    {
        QByteArray line = file.readLine();
        QList<QByteArray> split = line.split(';');

        mPalette[i++] = qRgb(split[0].toInt(), split[1].toInt(), split[2].toInt());
    }

    return i == 256;
}

void GeneratorLink::ResizeFromEvent(int width, int height)
{
    if (width < glb::constants::MIN_WIDTH || height < glb::constants::MIN_HEIGHT) return;

    double ratio = glb::constants::DEFAULT_RATIO;

    // Fixed ratio
    if (width < height * ratio)
    {
        mWidth = width;
        mHeight = width / ratio;
    }
    else
    {
        mWidth = height * ratio;
        mHeight = height;
    }

    mWidthText->setText(QString::number(mWidth));
    mHeightText->setText(QString::number(mHeight));
    mCurrentImage = QImage{mWidth, mHeight, QImage::Format_RGB32};
}

void GeneratorLink::handlePalette(QString filename){
    if (filename == "Default") LoadDefaultPalette();
    else if (!LoadPalette(glb::constants::DEFAULT_PALETTE_DIR.filePath(filename))) return;

    mCurrentImage = mCurrentResult.ToImage(mPalette, sizeof(mPalette)/sizeof(*mPalette));
    mConnectedScreen->addPixmap(QPixmap::fromImage(mCurrentImage));
}

void GeneratorLink::LoadDefaultPalette() {
    for (int i = 0; i < 256; ++i) mPalette[i] = qRgb(i,i,i);
}

void GeneratorLink::saveImage()
{
    QFileDialog dialog(nullptr, "Save as", QString() ,"PNG file (*.png)");
    dialog.setDefaultSuffix(".png");
    dialog.setAcceptMode(QFileDialog::AcceptSave);

    if (dialog.exec())
    {
        const auto fn = dialog.selectedFiles().at(0);
        mCurrentImage.save(fn);
    }
}

