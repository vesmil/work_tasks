#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QDir>

namespace glb
{
    namespace constants
    {
        static const QDir DEFAULT_PALETTE_DIR = QDir("../fractal_palettes");

        static const int MIN_HEIGHT = 50;
        static const int MIN_WIDTH = 75;

        static const int DEFAULT_WIDTH = 900;
        static const int DEFAULT_HEIGHT = 600;

        static const int DEFAULT_ALGO_DEPTH = 50;

        static const double X_MIN = -2.0;
        static const double X_MAX = 1.0;
        static const double Y_MIN = -1.0;
        static const double Y_MAX = 1.0;

        static const double DEFAULT_RATIO = (X_MAX - X_MIN) / (Y_MAX - Y_MIN);

        static const bool USE_SYMMETRICITY = true;
    }
}

#endif // CONSTANTS_H
