#ifndef STYLESHEETCREATOR_H
#define STYLESHEETCREATOR_H

#include <QColor>

// To eliminate need for magic strings
// used as `->setStyleSheet(SheetCreator:: ... )`

class SheetCreator {
public:
    static QString GetBackground(QColor color) {
        return "background-color: " + color.name();
    }

    static QString GetColor(QColor color) {
        return "color: " + color.name();
    }
};

#endif // STYLESHEETCREATOR_H
