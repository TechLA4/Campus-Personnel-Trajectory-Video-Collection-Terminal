#ifndef SWITCHDATABASE_H
#define SWITCHDATABASE_H

#include <QWidget>

namespace Ui {
class switchdatabase;
}

class switchdatabase : public QWidget
{
    Q_OBJECT

public:
    explicit switchdatabase(QWidget *parent = nullptr);
    ~switchdatabase();

private:
    Ui::switchdatabase *ui;
};

#endif // SWITCHDATABASE_H
