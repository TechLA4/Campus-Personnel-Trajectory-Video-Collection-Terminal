#include "database.h"
#include "ui_database.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <iostream>

database::database(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::database)
{
    ui->setupUi(this);

}

database::~database()
{
    delete ui;
}

void database::on_pushButton_clicked()
{
     emit sendsignal();
}


bool database::openFile()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setOption(QFileDialog::DontUseNativeDialog);
    dialog.setNameFilter("All Files (*)");
    dialog.setGeometry(QRect(0, 100, 200, 600));     // 设置合适尺寸

    if (!dialog.exec()) return false;

    QStringList files = dialog.selectedFiles();
    if (files.isEmpty())
    {
        QMessageBox::information(this, "Information", "fuck");
        return false;
    }


    QString fileName = files.first();
    QFileInfo fileInfo(fileName);

    // 新增验证：确保选择的是文件而非目录
    if (!fileInfo.isFile()) {
        QMessageBox::warning(this, "Error", "Selected path is a directory");
        return false;
    }

    file.setFileName(fileName);

    // 优化文件打开逻辑
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", QString("Cannot open file: %1").arg(file.errorString()));
        return false;
    }

    // 添加作用域限制文件读取
    {
        QTextStream stream(&file);
        QString content = stream.readAll();
        if (stream.status() != QTextStream::Ok) {
            QMessageBox::critical(this, "Error", "File read error");
            file.close();
            return false;
        }
        ui->textEdit->setPlainText(content);
        QMessageBox::information(this, "Information", content);
    }

    file.close();


    return true;
}
void database::closeFile()
{
    /* 检测打开按钮是否可用，不可用时，说明已经打开了文件 */
    if (!openPushButton->isEnabled()) {

        /* 以只读的方式打开 */
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

        /* 用文本流读取文件 */
        QTextStream stream(&file);

        /* 获取textEdit的文本内容，转为字符串 */
        QString str = ui->textEdit->toPlainText();

        /* 使用流提取运算符，写入文本流 */
        stream<<str;

        /* 清空textEdit的显示内容 */
        ui->textEdit->clear();


        /* 关闭文件 */
        file.close();

        /* 重新设置打开和关闭按钮的属性 */
        openPushButton->setEnabled(true);
        closePushButton->setEnabled(false);
    }
}


void database::on_pushButton_2_clicked()
{
    openFile();
}

void database::on_pushButton_3_clicked()
{
    closeFile();
}
