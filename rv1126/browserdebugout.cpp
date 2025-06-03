#include <QTextBrowser>
#include <QIODevice>

class BrowserDebugOut : public QIODevice
{
    Q_OBJECT

public:
    BrowserDebugOut(QTextBrowser *browser) : QIODevice(), m_browser(browser) {}

protected:
    long long writeData(const char *data, qint64 maxSize) override
    {
        m_browser->insertPlainText(QString::fromUtf8(data, maxSize));
    }

private:
    QTextBrowser *m_browser;
};
