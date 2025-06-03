#ifndef logout_H
#define logout_H

#include <QWidget>
#include <QTextEdit>
#include <QFile>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

namespace Ui {
class logout;
}

class logout : public QWidget
{
    Q_OBJECT

public:
    explicit logout(QWidget *parent = nullptr);
    ~logout();

private slots:
    void on_pushButton_clicked();

signals:
    void sendsignal();//这个函数用户向主界面通知关闭的消息

private:
    Ui::logout *ui;

 private slots:

       /* 打开文本文件 */
       bool openFile();

       /* 关闭文本文件 */
       void closeFile();
       void on_pushButton_2_clicked();
       void on_pushButton_3_clicked();
};

#endif // logout_H
