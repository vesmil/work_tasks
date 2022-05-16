#include "calculator.h"

Calculator::Calculator(QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(WIDTH, HEIGHT);
    ButtonsInit();
    DisplayInit();
}

void Calculator::DisplayInit() {
    LabelInit(prev_num_display, OFFSET_X, DISPLAY_OFFSET_Y, WIDTH - 2 * OFFSET_X,
              BUTTON_OFFSET_Y - 2 * DISPLAY_OFFSET_Y, Qt::AlignmentFlag::AlignRight);

    LabelInit(display, OFFSET_X, DISPLAY_OFFSET_Y + prev_num_display->heightMM() + DISPLAY_SPACE_SIZE,
              WIDTH - 2 * OFFSET_X, BUTTON_OFFSET_Y - 2 * DISPLAY_OFFSET_Y, Qt::AlignmentFlag::AlignRight);

    LabelInit(operation_label, OFFSET_X, DISPLAY_OFFSET_Y + prev_num_display->heightMM() + DISPLAY_SPACE_SIZE,
              OFFSET_X, BUTTON_OFFSET_Y - 2 * DISPLAY_OFFSET_Y, Qt::AlignmentFlag::AlignLeft);

    UpdateDisplay();
}

void Calculator::LabelInit(std::shared_ptr<QLabel> label, int pos_x, int pos_y, int width, int height, Qt::AlignmentFlag align)
{
    label->setGeometry(pos_x, pos_y, width, height);
    label->setAlignment(align);
    label->setFont(FONT);
}

void Calculator::ButtonsInit() {
    NumButtonsInit();
    OperationButtonsInit();
    OtherButtonsInit();
}

void Calculator::NumButtonsInit()
{
    for (int x = 0; x < 10; ++x) {
        NumButton *button = new NumButton((x + 1) % 10, this);
        SetButtonPosOnGrid(x, 3, *button);
        connect(button, SIGNAL (buttonClicked(int)), this, SLOT (NumPress(int)));
    }
}

void Calculator::OperationButtonsInit()
{
    enum OperationButton::operation op_operations[] = {OperationButton::add, OperationButton::subtract, OperationButton::multiply, OperationButton::divide};
    int op_position[]   = {3,8,13,18};

    for (int x = 0; x < 4; ++x) {
        OperationButton *button = new OperationButton(op_operations[x],OpToString(op_operations[x]), this);
        SetButtonPosOnGrid(op_position[x], 5, *button);
        connect(button, SIGNAL (buttonClicked(int)), this, SLOT (OperatorPress(int)));
    }
}

void Calculator::OtherButtonsInit()
{
    QString button_symbols[] = {"=",".",ANS,DEL,DEL_ALL};
    int button_positions[]   = {17,16,4,9,14};
    GeneralButton* buttons[5];

    for (int x = 0; x < 5; ++x) {
        GeneralButton *button = new GeneralButton(button_symbols[x], this);
        SetButtonPosOnGrid(button_positions[x], 5, *button);
        buttons[x] = button;
        if (x > 1) button->setStyleSheet(SheetCreator::GetBackground(OTHERS_COLOR));
    }

    connect(buttons[0], &QPushButton::clicked, this, &Calculator::Equals);
    connect(buttons[1], &QPushButton::clicked, this, &Calculator::InsertDecimal);
    connect(buttons[2], &QPushButton::clicked, this, &Calculator::LoadPrevious);
    connect(buttons[3], &QPushButton::clicked, this, &Calculator::DeleteLast);
    connect(buttons[4], &QPushButton::clicked, this, &Calculator::DeleteAll);
}

void Calculator::SetButtonPosOnGrid(int position, int mod, QPushButton &button)
{
    button.setGeometry(OFFSET_X + position % mod * (BUTTON_WIDTH + SPACE_SIZE),
                       BUTTON_OFFSET_Y + position / mod * (BUTTON_HEIGHT + SPACE_SIZE),
                       BUTTON_WIDTH, BUTTON_HEIGHT);
}
