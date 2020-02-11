#ifndef TABLELINEPUSHBUTTON_H
#define TABLELINEPUSHBUTTON_H
#include <QPushButton>

class TableLinePushButton : public QPushButton
{
    Q_OBJECT

    public:
        TableLinePushButton();
        ~TableLinePushButton();
        int row;
    };

#endif // TABLELINEPUSHBUTTON_H
