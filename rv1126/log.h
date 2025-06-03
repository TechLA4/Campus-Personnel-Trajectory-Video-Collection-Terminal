#ifndef LOG_H
#define LOG_H

#include <QWidget>

namespace Ui {
class Log;
}

class Log : public QWidget
{
    Q_OBJECT

public:
    explicit Log(QWidget *parent = nullptr);
    ~Log();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Log *ui;

    /* 打开文本文件 */
    bool openFile();

    /* 关闭文本文件 */
    void closeFile();
};

#endif // LOG_H
