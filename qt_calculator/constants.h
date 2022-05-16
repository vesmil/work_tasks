#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QFont>
#include <qcolor.h>

static const int HEIGHT = 310;
static const int WIDTH = 300;
static const int BUTTON_HEIGHT = 50;
static const int BUTTON_WIDTH = 50;
static const int OFFSET_X = 17;
static const int BUTTON_OFFSET_Y = 80;
static const int DISPLAY_OFFSET_Y = 10;
static const int SPACE_SIZE = 4;
static const int DISPLAY_SPACE_SIZE = 12;

static const QColor OPERATIONS_COLOR{209,119,52};
static const QColor OTHERS_COLOR{176,176,176};

static const QColor BLACK{0,0,0};
static const QColor LIGHT_GREY{169,169,169};

static const QFont FONT{"Arial", 18, 0};

static const QString ANS = "ans";
static const QString DEL = "del";
static const QString DEL_ALL = "DEL";

#endif // CONSTANTS_H
