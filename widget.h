#ifndef WIDGET_H
#define WIDGET_H

#include <QNetworkReply>
#include <QWidget>
#include <qboxlayout.h>
#include <qlabel.h>
#include <qlineedit.h>

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
    QLineEdit* codeuser;
    QLabel* customsFees;
    QVBoxLayout* vatVariants;
    QLineEdit* price;
    QString importValue;
    QLabel* calculation;

private slots:
    void onClicked();
    void onFinished(QNetworkReply* reply);
    void onCalculate();

};
#endif // WIDGET_H
