#include "eurokg.h"
#include "currency.h"
#include "ui_eurokg.h"

Eurokg::Eurokg(const QDomElement& import)
{
    ui = new Ui::Eurokg;
    ui->setupUi(this);
    valueCount =import.firstChildElement("ValueDetail").firstChildElement("ValueCount").text().toFloat();
    ui->info->setText(import.firstChildElement("Value").text());

}

Eurokg::~Eurokg()
{
    delete ui;
}

float Eurokg::calculate(Fees fees)
{
    qDebug() << Currency::get().euro();
    qDebug() <<Currency::get().dollar();

    float euroKgFees = valueCount * Currency::get().euro()/Currency::get().dollar()*ui->inputKg->value();
    return euroKgFees;
}

