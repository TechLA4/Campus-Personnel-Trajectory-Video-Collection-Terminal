#include "switchdatabase.h"
#include "ui_switchdatabase.h"

switchdatabase::switchdatabase(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::switchdatabase)
{
    ui->setupUi(this);
}

switchdatabase::~switchdatabase()
{
    delete ui;
}
