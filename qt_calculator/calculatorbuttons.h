#ifndef CALCULATORBUTTONS_H
#define CALCULATORBUTTONS_H

#include <QPushButton>
#include <QWidget>

#include "styleSheetCreator.h"
#include "constants.h"

class NumButton : public QPushButton {
Q_OBJECT
public:
    NumButton(const int num, QWidget *parent = nullptr) :QPushButton(QString::number(num), parent), number(num) {
        connect(this, SIGNAL(clicked()), this, SLOT(handleClick()));
    }

signals:
    void buttonClicked(int);
private slots:
    void handleClick() {
        emit buttonClicked(number);
    }

private:
    int number;
};

class OperationButton : public QPushButton {
Q_OBJECT
public:
    enum operation {none, add, subtract, multiply, divide};

    OperationButton(const enum operation op, QString symbol , QWidget *parent = nullptr) : QPushButton(symbol, parent), button_op(op) {
        connect(this, SIGNAL(clicked()), this, SLOT(handleClick()));
        setStyleSheet(SheetCreator::GetBackground(OPERATIONS_COLOR));
    }

signals:
    void buttonClicked(int);
private slots:
    void handleClick() {
        emit buttonClicked((int) button_op);
    }

private:
    enum operation button_op;
};

// Just to provide the same interface
class GeneralButton : public QPushButton {
Q_OBJECT
public:
    GeneralButton(QString label, QWidget *parent = nullptr) :QPushButton(label, parent) {
        connect(this, SIGNAL(clicked()), this, SLOT(handleClick()));
    }

signals:
    void buttonClicked();
private slots:
    void handleClick() {
        emit buttonClicked();
    }
};

#endif // CALCULATORBUTTONS_H
