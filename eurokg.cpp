#include "eurokg.h"
#include "ui_eurokg.h"

Eurokg::Eurokg(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Eurokg)
{
    ui->setupUi(this);
}

Eurokg::~Eurokg()
{
    delete ui;
}

float Eurokg::calculate(Fees fees)
{

}
