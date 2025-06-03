#include "config.h"
#include "ui_form1.h"

form1::form1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::form1)
{
    ui->setupUi(this);
}

form1::~form1()
{
    delete ui;
}
