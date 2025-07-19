#ifndef EUROKG_H
#define EUROKG_H
#include "icustomfees.h"
#include <QDomElement>
#include <QWidget>

namespace Ui {
class Eurokg;
}

class Eurokg : public QWidget, public ICustomFees
{
    Q_OBJECT

public:
    Eurokg(const QDomElement& import);
    ~Eurokg();
    float calculate(Fees fees) override;
    float valueCount;

private:
    Ui::Eurokg *ui;
};

#endif // EUROKG_H
