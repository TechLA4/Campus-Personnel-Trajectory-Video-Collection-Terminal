#include "log.h"
#include "ui_log.h"

Log::Log(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Log)
{
    ui->setupUi(this);
}

Log::~Log()
{
    delete ui;
}

void Log::on_pushButton_2_clicked()
{

}

void Log::on_pushButton_clicked()
{

}

void Log::on_pushButton_3_clicked()
{

}
