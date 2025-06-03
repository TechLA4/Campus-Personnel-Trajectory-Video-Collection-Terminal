#include "database_inf.h"
#include "ui_database_inf.h"
#include <QFileDialog>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlError>
#include <QMessageBox>



database_inf::database_inf(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::database_inf)
{
    ui->setupUi(this);



}

database_inf::~database_inf()
{
    delete ui;
}

void database_inf::on_pushButton_clicked()
{
       emit sendsignal();
}
void database_inf::openDB()
{



    // 设置数据库路径
    QString dbPath = "/demo/bin/rockx_face_table.db3";
    // 检查文件是否存在
    if(!QFile::exists(dbPath)) {
        QMessageBox::critical(this, "错误", "数据库文件不存在: " + dbPath);
        return;
    }


    // 打开数据库
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    if(!db.open()) {
        QMessageBox::critical(this, "错误", "无法打开数据库: " + db.lastError().text());
        return;
    }

    // 创建模型并设置到tableView
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("SELECT * FROM face_table");

    if(model->lastError().isValid()) {
        QMessageBox::critical(this, "错误", "查询错误: " + model->lastError().text());
        return;
    }

    // 设置列标题名称（可选）
    model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, tr("姓名"));
    model->setHeaderData(2, Qt::Horizontal, tr("性别"));
    model->setHeaderData(3, Qt::Horizontal, tr("专业"));
    model->setHeaderData(4, Qt::Horizontal, tr("人脸特征值"));
    model->setHeaderData(5, Qt::Horizontal, tr("图片URL"));

    ui->tableView->setModel(model);

    // 获取当前表头字体
    QFont headerFont = ui->tableView->horizontalHeader()->font();
    // 设置表头字体大小（例如设为10号）
    headerFont.setPointSize(9);
    // 应用新字体
    ui->tableView->horizontalHeader()->setFont(headerFont);

    // 设置整个tableView的字体大小
    ui->tableView->setStyleSheet("QTableView { font-size: 7pt; }");

    // 设置所有单元格内容字体为7pt
    QFont contentFont;
    contentFont.setPointSize(7);  // 设置为7磅
    ui->tableView->setFont(contentFont);

    ui->tableView->setStyleSheet(
        "QScrollBar:horizontal { height: 50px; }"
        "QScrollBar:vertical { width: 20px; }"
    );


    // 设置列宽和显示属性
    ui->tableView->setColumnWidth(0, 100);    // ID列
    ui->tableView->setColumnWidth(1, 100);    // 姓名列
    ui->tableView->setColumnWidth(2, 100);    // 性别列
    ui->tableView->setColumnWidth(3, 120);   // 专业列
    ui->tableView->setColumnWidth(4, 300);   // 特征值列（设置较宽但仍有限制）
    ui->tableView->setColumnWidth(5, 300);   // URL列（设置较宽但仍有限制）

    // 设置文本过长时显示省略号
    ui->tableView->setTextElideMode(Qt::ElideRight);

    ui->tableView->setWordWrap(false); // 全局禁用换行

    // 特别设置特征值和URL列的最大宽度
    ui->tableView->horizontalHeader()->setMaximumSectionSize(300);  // 设置最大宽度

    QMessageBox::information(this, "成功", "数据库已自动加载");

    ui->label->setFrameShape(QFrame::Box);
    ui->label->setText("点击Name查看图片");

    connect(ui->tableView, &QTableView::clicked, [this](const QModelIndex &index){
        qDebug() << "点击行:" << index.row() << "列:" << index.column();

        // 获取列名
        QString columnName = ui->tableView->model()->headerData(index.column(), Qt::Horizontal).toString();
        qDebug() << "点击列名:" << columnName;

        if(columnName.compare("姓名", Qt::CaseInsensitive) == 0 ||
           columnName.compare("name", Qt::CaseInsensitive) == 0) {
            // 获取同一行的url列数据
            QModelIndex urlIndex = index.sibling(index.row(), 5); // 假设url是第6列(索引为5)
            QString imageUrl = urlIndex.data().toString();

            qDebug() << "获取到的URL:" << imageUrl;

            // 添加URL有效性检查
            if(imageUrl.isEmpty()) {
                qDebug() << "URL为空!";
                ui->label->setText("URL为空，无法加载图片");
                return;
            }

            if(!imageUrl.startsWith("http://") && !imageUrl.startsWith("https://")) {
                qDebug() << "URL格式不正确!";
                ui->label->setText("URL格式不正确，必须以http://或https://开头");
                return;
            }

            displayImage(imageUrl, ui->label);
        }
    });










}




void database_inf::on_pushButton_2_clicked()
{
    openDB();
}


void database_inf::on_tableView_activated(const QModelIndex &index)
{
    printf("activate");
}



// 显示图片的函数
void database_inf::displayImage(const QString &url, QLabel *label)
{
    qDebug() << "尝试加载图片URL:" << url;

    label->clear();
    label->setText("加载图片中...");
    QApplication::processEvents();

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    // 添加SSL错误处理
    connect(manager, &QNetworkAccessManager::sslErrors, [](QNetworkReply *reply, const QList<QSslError> &errors){
        qDebug() << "SSL错误发生:";
        for(const QSslError &error : errors) {
            qDebug() << "SSL错误:" << error.errorString();
        }
        reply->ignoreSslErrors(); // 忽略SSL错误（仅用于测试，生产环境不推荐）
    });

    connect(manager, &QNetworkAccessManager::finished, [label, url](QNetworkReply *reply){
        qDebug() << "图片加载完成，URL:" << url << "错误:" << reply->error();

        if(reply->error() == QNetworkReply::NoError) {
            QByteArray imageData = reply->readAll();
            QPixmap pixmap;
            if(pixmap.loadFromData(imageData)) {
                qDebug() << "图片尺寸:" << pixmap.size();
                pixmap = pixmap.scaled(label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                label->setPixmap(pixmap);
            } else {
                qDebug() << "图片数据加载失败";
                label->setText("图片数据加载失败");
            }
        } else {
            qDebug() << "网络错误:" << reply->errorString();
            label->setText("加载失败: " + reply->errorString());
        }
        reply->deleteLater();
    });

    // 正确的请求创建方式
        QNetworkRequest request;
        request.setUrl(QUrl(url));
        // 设置一些请求头，模拟浏览器请求
        request.setRawHeader("User-Agent", "Mozilla/5.0");
        manager->get(request);
}
