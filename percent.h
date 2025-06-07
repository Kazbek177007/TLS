#ifndef PERCENT_H
#define PERCENT_H

#include <QDomDocument>
#include <QLabel>
#include <QObject>

class Percent : public QLabel
{
    Q_OBJECT
public:
    Percent(QDomElement imports);
    QString importValue;
};

#endif // PERCENT_H
