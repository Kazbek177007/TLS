#include "widget.h"
#include "ui_widget.h"

#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    auto vbox = new QVBoxLayout;
    setLayout(vbox);
    auto hbox = new QHBoxLayout;
    vbox->addLayout(hbox);
    auto confirm = new QPushButton("Confirm");
    vbox->addWidget(confirm);
    auto customsFees = new QLabel("Customs fees");
    vbox->addWidget(customsFees);
    auto calculation = new QLabel;
    calculation->setText("formula");
    vbox->addWidget(calculation);
    hbox->addWidget(new QLineEdit);
    hbox->addWidget(new QComboBox);
    hbox->addWidget(new QLineEdit);
    networkManager = new QNetworkAccessManager;
    connect(confirm,&QPushButton::clicked,
            this, &Widget::onClicked);

}

Widget::~Widget()
{
    delete ui;
    delete networkManager;
}

void Widget::onClicked()
{

}

void Widget::onFinished(QNetworkReply *reply)
{

}
