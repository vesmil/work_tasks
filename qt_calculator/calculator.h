#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QKeyEvent>
#include <QLabel>
#include <QPushButton>
#include <QWidget>

#include "calculatorbuttons.h"
#include "constants.h"
#include "calculatorNumber.h"

class Calculator : public QWidget
{
    Q_OBJECT

public:
    explicit Calculator(QWidget *parent = nullptr);

public slots:
    void NumPress(int num);
    void InsertDecimal();
    void OperatorPress(int op);

private:
    void ButtonsInit();
    void NumButtonsInit();
    void OperationButtonsInit();
    void OtherButtonsInit();
    void SetButtonPosOnGrid(int position, int mod, QPushButton &button);

    void DisplayInit();
    void LabelInit(std::shared_ptr<QLabel> label, int pos_x, int pos_y, int width, int height, enum Qt::AlignmentFlag align);

    void Equals();
    void LoadPrevious();
    void DeleteLast();
    void DeleteAll();

    void UpdateDisplay();

    QString OpToString(enum OperationButton::operation op);
    enum OperationButton::operation current_operation = OperationButton::operation::none;

    CalculatorNumber current_number{};
    CalculatorNumber prev_number{};

    std::shared_ptr<QLabel> display = std::make_shared<QLabel>(this);
    std::shared_ptr<QLabel> prev_num_display = std::make_shared<QLabel>(this);
    std::shared_ptr<QLabel> operation_label = std::make_shared<QLabel>(this);

protected:
    void keyPressEvent( QKeyEvent * event ) override;
};

#endif
