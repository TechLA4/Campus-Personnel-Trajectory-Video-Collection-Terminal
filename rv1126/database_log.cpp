#include "database_log.h"
#include "ui_database_log.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <iostream>


database_log::database_log(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::database_log)
{
    ui->setupUi(this);
}

database_log::~database_log()
{
    delete ui;
}

void database_log::on_pushButton_clicked()
{
    emit sendsignal();
}

void database_log::on_pushButton_2_clicked()
{
    openFile();
}

void database_log::on_pushButton_3_clicked()
{
    closeFile();
}

bool database_log::openFile()
{
    // 创建文件对话框实例
    QFileDialog dialog(this);

    // 设置对话框属性
    dialog.setWindowTitle("Open File");
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setOption(QFileDialog::DontUseNativeDialog); // 关键选项
    dialog.setGeometry(QRect(-280, 150, 600, 800));     // 设置合适尺寸
    dialog.setStyleSheet(
        "QScrollBar:vertical { width: 50px; }"
        "QScrollBar:horizontal { height: 50px; }"
    );

    // 执行对话框
    if (!dialog.exec())
        return false;

    // 获取选中文件
    QStringList files = dialog.selectedFiles();
    if (files.isEmpty())
        return false;

    QString fileName = files.first();
    // 声明局部变量 QFile
    QFile file(fileName);  // 使用构造函数直接设置文件名

    /* 后续原有代码保持 */
    file.setFileName(fileName);

    if (!file.exists())
        return false;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    ui->textEdit_2->setPlainText(stream.readAll());
    file.close();

    return true;
}



void database_log::closeFile()
{
    QFile file;
    /* 检测打开按钮是否可用，不可用时，说明已经打开了文件 */
    if (!ui->pushButton_3->isEnabled()) {

        /* 以只读的方式打开 */
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

        /* 用文本流读取文件 */
        QTextStream stream(&file);

        /* 获取textEdit的文本内容，转为字符串 */
        QString str = ui->textEdit_2->toPlainText();

        /* 使用流提取运算符，写入文本流 */
        stream<<str;

        /* 清空textEdit的显示内容 */
        ui->textEdit->clear();


        /* 关闭文件 */
        file.close();


    }
}
