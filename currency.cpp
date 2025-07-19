#include "currency.h"

#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <iconv.h>



Currency::Currency()
{
    QNetworkAccessManager manager;
    QEventLoop loop;
    QNetworkReply* reply = manager.get(QNetworkRequest(QUrl("https://www.cbr.ru/scripts/XML_daily.asp")));
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    const QByteArray data = reply->readAll();

    iconv_t cd = iconv_open("UTF-8","CP1251");
    char* inbuf = const_cast<char*>(data.data());
    size_t insize = data.size();
    size_t outsize = insize*2;
    QByteArray outData(outsize,0);
    char* outbuf = outData.data();
    iconv(cd,&inbuf,&insize, &outbuf, &outsize);
    iconv_close(cd);

    outData.replace("windows-1251","utf-8");
    QDomDocument doc;
    auto parseResult = doc.setContent(outData);
    if (!parseResult)
    {
        qDebug() << parseResult.errorMessage;
    }
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
            return value.text().replace(",", ".").toFloat();
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
            return value.text().replace(",", ".").toFloat();
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
            return value.text().replace(",", ".").toFloat();
        }
    }
    return 0.0;
}
