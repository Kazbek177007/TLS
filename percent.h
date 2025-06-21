#ifndef PERCENT_H
#define PERCENT_H

#include "icustomfees.h"

#include <QDomDocument>
#include <QLabel>
#include <QObject>

class Percent : public QLabel, public ICustomFees
{
    Q_OBJECT
public:
    Percent(QDomElement import);
    QString importValue;
    float calculate(Fees fees) override;
    int value;
};

#endif // PERCENT_H
