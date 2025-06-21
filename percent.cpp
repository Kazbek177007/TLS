#include "percent.h"
#include <QDomDocument>
#include <QDomElement>
#include <QDebug>
#include <QTextStream>

void printDomElement(const QDomElement& element) {
    QString xml;
    QTextStream stream(&xml);
    element.save(stream, 4);
    qDebug().noquote() << xml;
}

Percent::Percent(QDomElement import)
{
    QDomElement value = import.firstChildElement("Value");
    setText(value.text());
    QDomElement valuechar = import.firstChildElement("ValueDetail").firstChildElement("ValueCount");
    Percent::value = valuechar.text().toInt();
}

float Percent::calculate(Fees fees)
{
    float respercent = fees.price * value/100;
    float resvat = (respercent + fees.price) * fees.vat/100;
    return resvat + respercent;
}

