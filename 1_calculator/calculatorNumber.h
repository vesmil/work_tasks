#ifndef CALCULATORNUMBER_H
#define CALCULATORNUMBER_H

#include <QString>

// Representation of number in calculator
// Working with string which evaluated afterwards (because of precision problem)

class CalculatorNumber {
public:
    double GetNumValue() {
        return value.toDouble();
    }

    CalculatorNumber DeepCopy(){
        CalculatorNumber temp;
        temp.value = value;
        temp.decimal_present = decimal_present;
        return temp;
    }

    void Clear(){
    value = QString{};
    decimal_present = false;
    }

    bool IsEmpty(){
        return value.length() == 0;
    }

    QString value = "";
    bool decimal_present = false;
};

#endif // CALCULATORNUMBER_H
