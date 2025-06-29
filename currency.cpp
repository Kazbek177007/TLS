#include "currency.h"

#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>



Currency::Currency()
{
    QNetworkAccessManager manager;
    QEventLoop loop;
    QNetworkReply* reply = manager.get(QNetworkRequest(QUrl("https://www.cbr.ru/scripts/XML_daily.asp")));
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    QDomDocument doc;
    doc.setContent(reply->readAll());
    root=doc.documentElement();
}

Currency &Currency::get()
{
    static Currency currency;
    return currency;
}

float Currency::euro() const
{
    QDomNodeList valutes = root.elementsByTagName("Valute");
    for (int i = 0; i < valutes.count(); i++) {
        QDomElement valute = valutes.at(i).toElement();
        QDomElement charCode = valute.firstChildElement("CharCode");
        if (charCode.text() == "EUR") {
            QDomElement value = valute.firstChildElement("Value");
            return value.text().replace(",", ".").toDouble();
        }
    }
    return 0.0;
}

float Currency::dollar() const
{
    QDomNodeList valutes = root.elementsByTagName("Valute");
    for (int i = 0; i < valutes.count(); i++) {
        QDomElement valute = valutes.at(i).toElement();
        QDomElement charCode = valute.firstChildElement("CharCode");
        if (charCode.text() == "USD") {
            QDomElement value = valute.firstChildElement("Value");
            return value.text().replace(",", ".").toDouble();
        }
    }
    return 0.0;
}

float Currency::yuan() const
{
    QDomNodeList valutes = root.elementsByTagName("Valute");
    for (int i = 0; i < valutes.count(); i++) {
        QDomElement valute = valutes.at(i).toElement();
        QDomElement charCode = valute.firstChildElement("CharCode");
        if (charCode.text() == "CNY") {
            QDomElement value = valute.firstChildElement("Value");
            return value.text().replace(",", ".").toDouble();
        }
    }
    return 0.0;
}
