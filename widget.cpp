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
    auto calculation = new QLabel;
    calculation->setText("formula");
    vbox->addWidget(calculation);
    codeuser = new QLineEdit;
    hbox->addWidget(codeuser);
    hbox->addWidget(new QComboBox);
    hbox->addWidget(new QLineEdit);
    networkManager = new QNetworkAccessManager;
    connect(confirm,&QPushButton::clicked,
            this, &Widget::onClicked);
    connect(networkManager, &QNetworkAccessManager::finished,
            this, &Widget::onFinished);
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
    QString importValue = root.firstChildElement("Importlist")
                              .firstChildElement("Import")
                              .firstChildElement("Value").text();
    resultText += "Импортная пошлина: " + importValue + "\n\n";

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
