#ifndef WIDGET_H
#define WIDGET_H

#include <QNetworkReply>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    QNetworkAccessManager* networkManager;
private slots:
    void onClicked();
    void onFinished(QNetworkReply* reply);
};
#endif // WIDGET_H
