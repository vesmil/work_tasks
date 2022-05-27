#include <QPushButton>

#include "calculator.h"

void Calculator::OperatorPress(int op)
{
    Equals();
    current_operation = (enum OperationButton::operation) op;
    operation_label->setText(OpToString(current_operation));
}

void Calculator::Equals()
{
    if (current_number.IsEmpty()) return;

    double first = prev_number.GetNumValue();
    double second = current_number.GetNumValue();

    prev_number.Clear();
    current_number.Clear();

    switch (current_operation) {
        case OperationButton::none:
            prev_number.value = QString::number(second);
            break;
        case OperationButton::add:
            prev_number.value = QString::number(first + second);
            break;
        case OperationButton::subtract:
            prev_number.value = QString::number(first - second);
            break;
        case OperationButton::multiply:
            prev_number.value = QString::number(first * second);
            break;
        case OperationButton::divide:
            prev_number.value = QString::number(first / second);
            break;
    }

    prev_number.decimal_present = fmod(prev_number.GetNumValue(),1)!= 0;

    current_operation = OperationButton::none;
    UpdateDisplay();
}

QString Calculator::OpToString(enum OperationButton::operation op) {
    switch (op) {
        case OperationButton::none:
            break;
        case OperationButton::add:
            return "+";
        case OperationButton::subtract:
            return "-";
        case OperationButton::multiply:
            return "*";
        case OperationButton::divide:
            return "/";
    }
    return "";
}

void Calculator::LoadPrevious()
{
    current_number = prev_number.DeepCopy();

    UpdateDisplay();
}

void Calculator::DeleteLast()
{
    if (current_number.IsEmpty()) {
        if (prev_number.IsEmpty()) return;

        current_number = prev_number.DeepCopy();
        prev_number.Clear();
        current_operation = OperationButton::none;
        UpdateDisplay();

        return;
    }

    if (current_number.value.last(1) == '.') current_number.decimal_present = false;
    current_number.value.chop(1);

    UpdateDisplay();
}

void Calculator::DeleteAll()
{
    current_number.Clear();
    prev_number.Clear();
    current_operation = OperationButton::none;
    UpdateDisplay();
}

void Calculator::UpdateDisplay()
{
    if (current_number.IsEmpty()) prev_num_display->setStyleSheet(SheetCreator::GetColor(BLACK));
    else prev_num_display->setStyleSheet(SheetCreator::GetColor(LIGHT_GREY));

    operation_label->setText(OpToString(current_operation));
    display->setText(current_number.value);
    prev_num_display->setText(prev_number.value);
}

void Calculator::NumPress(int num)
{
    current_number.value += QString::number(num);
    UpdateDisplay();
}

void Calculator::InsertDecimal()
{
    if (current_number.decimal_present) return;
    if (current_number.IsEmpty()) current_number.value += QString("0");

    current_number.decimal_present = true;
    current_number.value += QString(".");

    UpdateDisplay();
}

void Calculator::keyPressEvent(QKeyEvent *event)
{
    if (event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9)
        NumPress(event->key() - Qt::Key_0);

    else switch( event->key() )
    {
        case Qt::Key_Plus:
            OperatorPress(OperationButton::add);
            break;
        case Qt::Key_Minus:
            OperatorPress(OperationButton::subtract);
            break;
        case Qt::Key_Asterisk:
            OperatorPress(OperationButton::multiply);
            break;
        case Qt::Key_Slash:
            OperatorPress(OperationButton::divide);
            break;

        case Qt::Key_Backslash:
            DeleteLast();
            break;

        case Qt::Key_Return:
        case Qt::Key_Enter:
            Equals();
            break;
    }
}
