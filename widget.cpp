#include "widget.h"
#include "ui_widget.h"

#include <QComboBox>
#include <QDomDocument>
#include <QFile>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
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
    customsFees = new QLabel("Customs fees");
    vbox->addWidget(customsFees);
    vatVariants = new QVBoxLayout;
    vbox->addLayout(vatVariants);
    auto calculate = new QPushButton("Calculate");
    vbox->addWidget(calculate);
    calculation = new QLabel;
    calculation->setText("formula");
    vbox->addWidget(calculation);
    codeuser = new QLineEdit;
    hbox->addWidget(codeuser);
    hbox->addWidget(new QComboBox);
    price = new QLineEdit;
    hbox->addWidget(price);
    networkManager = new QNetworkAccessManager;
    connect(confirm,&QPushButton::clicked,
            this, &Widget::onClicked);
    connect(networkManager, &QNetworkAccessManager::finished,
            this, &Widget::onFinished);
    connect(calculate, &QPushButton::clicked,
            this, &Widget::onCalculate);
}

Widget::~Widget()
{
    delete ui;
    delete networkManager;
}

void Widget::onClicked()
{
    QString code = codeuser->text();
    if (code.isEmpty())
        return;
    QString login = "testlogin";
    QString pass = "testpassword";
    QString password = QCryptographicHash::hash(pass.toUtf8(), QCryptographicHash::Md5).toHex();
    QString secret = QCryptographicHash::hash((code+":"+login+":"+password).toUtf8(), QCryptographicHash::Md5).toHex();
    QString countrycode = "156";

    const QUrl url("https://www.alta.ru/tnved/xml/?tncode="+code+"&login="+login+"&secret="+secret);
    QNetworkRequest request(url);
    networkManager->get(request);

    codeuser->clear();
}

void Widget::onFinished(QNetworkReply *reply)
{
    if(reply->error()!=QNetworkReply::NoError)
    {
        qDebug()<< "Error:" << reply->errorString();
        return;
    }
    QByteArray data = reply->readAll();

    QDomDocument doc;
    doc.setContent(data);

    QDomElement root = doc.documentElement();
    QString resultText;

    // 1. Code
    QString code = root.firstChildElement("Code").text();
    resultText += "Код: " + code + "\n";

    // 2. Name
    QString name = root.firstChildElement("Name").text();
    resultText += "Наименование: " + name + "\n\n";

    // 3. Import->Value
    importValue = root.firstChildElement("Importlist")
                              .firstChildElement("Import")
                              .firstChildElement("Value").text();
    resultText += "Импортная пошлина: " + importValue + "\n\n";
    importValue.chop(1);

    // 4. VAT data
    resultText += "НДС:\n";
    QDomNodeList vatNodes = root.firstChildElement("VATlist").elementsByTagName("VAT");
    customsFees->setText(resultText);

    for (int i = 0; i < vatNodes.size(); ++i) {
        QDomElement vatElement = vatNodes.at(i).toElement();
        QString vatValue = vatElement.firstChildElement("Value").text();
        QString vatCondition = vatElement.firstChildElement("Condition").text();
        QRadioButton* button = new QRadioButton(vatValue+vatCondition);
        vatVariants->addWidget(button);
    }

    reply->deleteLater();
}

void Widget::onCalculate()
{
    QRadioButton* selectedButton = nullptr;
    for (int i = 0; i<vatVariants->count(); i++)
    {
        QWidget* widget = vatVariants->itemAt(i)->widget();
        QRadioButton* ptrbutton = dynamic_cast<QRadioButton*>(widget);
        if (ptrbutton->isChecked())
        {
            selectedButton = ptrbutton;
            break;
        }

    }
    if (!selectedButton)
    {
        return;
    }
    QString selectedVat = selectedButton->text();
    selectedVat = selectedVat.split('%')[0];
    int vat = selectedVat.toInt();
    int imValue = importValue.toInt();
    bool ok;
    float price2 = price->text().toInt(&ok);
    if(!ok)
    {
        return;
    }
    float impValue = price2*(imValue/100.00);
    float resValue = price2+impValue;
    float result = (resValue)*(vat/100.00);
    float stp = result+impValue;
    calculation->setText(QString("Таможенный пошлина: (%1*%2%)=%3\n НДС: %4*%5%=%6\n СТП: %7").arg(price2).arg(imValue).arg(impValue).arg(resValue).arg(vat).arg(result).arg(stp));




    // for button()

    //delete qradiburron
}
