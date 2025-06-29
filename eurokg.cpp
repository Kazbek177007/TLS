#include "eurokg.h"
#include "currency.h"
#include "ui_eurokg.h"

Eurokg::Eurokg(const QDomElement& import)
{
    ui = new Ui::Eurokg;
    ui->setupUi(this);
    valueCount =import.firstChildElement("Value").firstChildElement("ValueDetail").firstChildElement("ValueCount").text().toFloat();

}

Eurokg::~Eurokg()
{
    delete ui;
}

float Eurokg::calculate(Fees fees)
{

    float euroKgFees = valueCount * Currency::get().euro()/Currency::get().dollar();
    return euroKgFees;
}

