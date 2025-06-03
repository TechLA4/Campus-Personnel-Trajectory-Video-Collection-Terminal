#ifndef DATABASE_LOG_H
#define DATABASE_LOG_H

#include <QWidget>
#include <QTextEdit>
#include <QFile>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

namespace Ui {
class database_log;
}

class database_log : public QWidget
{
    Q_OBJECT

public:
    explicit database_log(QWidget *parent = nullptr);
    ~database_log();

private slots:
    void on_pushButton_clicked();



    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

signals:
    void sendsignal();//这个函数用户向主界面通知关闭的消息

private:
    Ui::database_log *ui;
    /* 打开文本文件 */
    bool openFile();

    /* 关闭文本文件 */
    void closeFile();
};

#endif // DATABASE_LOG_H
