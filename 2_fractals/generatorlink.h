#ifndef GENERATORLINK_H
#define GENERATORLINK_H

#include <QGraphicsScene>
#include <QMutex>
#include <QWidget>
#include <QTextEdit>
#include <deque>

#include "fractalgenerator.h"

class GeneratorLink : public QObject
{
    Q_OBJECT

public:
    GeneratorLink(QGraphicsScene *scene, QTextEdit *widthText, QTextEdit *heightText, QTextEdit *depthText);
    virtual ~GeneratorLink() = default;

    void ResizeFromEvent(int width, int height);

    int step = 0;

public slots:
    void handleGenerate();
    void handlePalette(QString fileName);
    void saveImage();

    void imageGenerated(ResultMatrix result);

private:
    void LoadTextValues();
    bool LoadPalette(QFile&& file);
    void LoadDefaultPalette();

    ResultMatrix mCurrentResult;
    QImage mCurrentImage;

    QGraphicsScene* mConnectedScreen;

    QTextEdit *mWidthText;
    QTextEdit *mHeightText;
    QTextEdit *mDepthText;

    QRgb mPalette[256];

    int mWidth;
    int mHeight;
    int mDepth;
};

#endif // GENERATORLINK_H
